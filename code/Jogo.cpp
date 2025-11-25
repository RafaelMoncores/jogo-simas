#include "Jogo.hpp"
#include <iostream>
#include "Entidades/Ente.hpp"
#include "Entidades/Personagens/Jogador.hpp"
#include "Fases/FaseUm.hpp"
#include "Fases/FaseDois.hpp"
#include <fstream>
#include <sstream>
#include <map>

using Gerenciadores::GerenciadorGrafico;
using Entidades::Ente;
using Estados::Menu;
using Fases::Fase;

// Inicializa ponteiros e delega a configuração para o método inicializar()
Jogo::Jogo() :
    pGG(nullptr),
    pGE(nullptr),
    menu(),
    pFaseAtual(nullptr),
    estadoAtual(EstadoJogo::NoMenu)
{
    inicializar();
}

// Garante a persistência do ranking e limpeza de memória manual (Managers/Fase)
Jogo::~Jogo()
{
    salvar();
    delete GerenciadorGrafico::getInstance();
    if (pFaseAtual) delete pFaseAtual;
}

// Configura Singletons, define ouvintes iniciais e carrega o ranking do disco
void Jogo::inicializar()
{
    pGG = GerenciadorGrafico::getInstance();
    pGE = Gerenciadores::GerenciadorEventos::getInstance();

    Ente::setGerenciadorGrafico(pGG);
    pGE->setOuvinte(&menu);

    // Leitura e parse do arquivo de ranking (formato texto simples)
    // ref:https://stackoverflow.com/questions/46719183/c-using-ifstream-to-read-file
    // ref:https://www.geeksforgeeks.org/cpp/stringstream-c-applications/
    std::ifstream ifs("ranking.txt");
    if (ifs.is_open()) {
        std::string line;
        int section = 0; 
        while (std::getline(ifs, line)) {
            if (line.empty()) continue;
            if (line == "---") { section = 0; continue; }
            if (line == "RankingFase1") { section = 1; continue; }
            if (line == "RankingFase2") { section = 2; continue; }

            std::istringstream iss(line);
            std::string nome; int pts;
            if (!(iss >> nome >> pts)) continue;
            
            if (section == 1) rankingFase1.push_back({nome, pts});
            else if (section == 2) rankingFase2.push_back({nome, pts});
        }

        // Mantém apenas o Top 10 usando std::sort com funtor greater
        std::sort(rankingFase1.begin(), rankingFase1.end(), std::greater<RankingEntry>());
        if (rankingFase1.size() > 10) rankingFase1.resize(10);
        
        std::sort(rankingFase2.begin(), rankingFase2.end(), std::greater<RankingEntry>());
        if (rankingFase2.size() > 10) rankingFase2.resize(10);
        
        ifs.close();
    }
}

// Lê o arquivo de save state, identifica a fase e reconstrói o cenário
bool Jogo::carregarSave()
{
    std::ifstream ifs("save_state.txt");
    if (!ifs.is_open()) return false;

    std::string snapshot((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();
    if (snapshot.empty()) return false;

    // Parse do cabeçalho para instanciar a classe de Fase correta
    std::istringstream iss(snapshot);
    std::string line;
    int faseNum = -1;
    bool modoDois = false;

    if (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string key;
        lineStream >> key >> faseNum;
    }
    if (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string key;
        int v;
        lineStream >> key >> v;
        if (key == "modoDoisJogadores") modoDois = (v != 0);
    }

    if (faseNum < 1 || faseNum > 2) return false;

    Fase* proxima = nullptr;
    if (faseNum == 1) proxima = new Fases::FaseUm(this, modoDois);
    else if (faseNum == 2) proxima = new Fases::FaseDois(this, modoDois);
    else return false;

    if (pFaseAtual) delete pFaseAtual;
    pFaseAtual = proxima;
    
    // Configura a fase para receber dados (sem criar obstáculos padrões) e aplica o snapshot
    pFaseAtual->inicializarParaLoad(); 
    pFaseAtual->limparEntidadesDinamicas();
    aplicarSnapshotAFase(snapshot);

    return true;
}

// Itera sobre as linhas do snapshot e agrupa dados de entidades para reconstrução
void Jogo::aplicarSnapshotAFase(const std::string& snapshot)
{
    if (!pFaseAtual) return;

    std::istringstream iss(snapshot);
    std::string line;
    std::vector<std::string> block;

    while (std::getline(iss, line)) {
        if (line == "---") {
            if (!block.empty()) processarBlocoEntidade(block);
            block.clear();
        } else if (!line.empty()) {
            block.push_back(line);
        }
    }
    if (!block.empty()) processarBlocoEntidade(block);
}

// Factory Method: Reconstrói uma entidade específica baseada no bloco de texto lido
void Jogo::processarBlocoEntidade(const std::vector<std::string>& block)
{
    if (!pFaseAtual) return;

    std::string tipo;
    std::map<std::string, std::string> kv;

    // Parse chave-valor das propriedades da entidade
    for (const auto& l : block) {
        std::istringstream iss(l);
        std::string a, b;
        if (!(iss >> a)) continue;
        if (!(iss >> b)) { tipo = a; continue; }
        kv[a] = b;
    }

    // Restaura propriedades específicas dependendo do tipo da entidade
    if (tipo == "Jogador") {
        int id = 1;
        if (kv.find("idJogador") != kv.end()) id = std::stoi(kv["idJogador"]);
        
        Entidades::Personagens::Jogador* pj = (id == 2) ? pFaseAtual->getJogador2() : pFaseAtual->getJogador1();
        if (pj) {
            if (kv.count("spritePosX") && kv.count("spritePosY")) {
                pj->setPosition({std::stof(kv["spritePosX"]), std::stof(kv["spritePosY"])});
            }
            if (kv.count("pontos")) pj->setPontos(std::stof(kv["pontos"]));
            if (kv.count("vidas")) pj->setVidas(std::stoi(kv["vidas"]));
            if (kv.count("podePular")) pj->setPodePular(kv["podePular"] != "0");
            if (kv.count("completouFase")) pj->setCompletouFase(kv["completouFase"] != "0");
            if (kv.count("direcao")) pj->setDirecao(std::stoi(kv["direcao"]));
            if (kv.count("velX") && kv.count("velY")) {
                pj->setVelocidade({std::stof(kv["velX"]), std::stof(kv["velY"])});
            }
        }
    }
    // Delegação para a fase restaurar inimigos e obstáculos
    else if (tipo == "Plataforma") pFaseAtual->restaurarPlataforma(kv);
    else if (tipo == "Trampolim") pFaseAtual->restaurarTrampolim(kv);
    else if (tipo == "BolaDeFogo") pFaseAtual->restaurarBolaDeFogo(kv);
    else if (tipo == "Vampiro") pFaseAtual->restaurarVampiro(kv);
    else if (tipo == "Gosma") pFaseAtual->restaurarGosma(kv);
    else if (tipo == "Dragao") pFaseAtual->restaurarDragao(kv);
}

// Loop principal do jogo (Pattern: Game Loop)
void Jogo::executar()
{
    while (pGG->isWindowOpen())
    {
        pGG->limpar(sf::Color::Black);
        float delta = relogio.restart().asSeconds();

        switch (estadoAtual)
        {
            // --- Estado: Menu Principal ---
            case EstadoJogo::NoMenu:
            {
                pGE->setOuvinte(&menu);
                pGE->processarEventos(); // Processa input para o menu
                
                int acaoMenu = menu.executar(rankingFase1, rankingFase2);
                Fases::Fase* proximaFase = nullptr;

                // Gerencia a seleção do menu (Sair, Fases, Load)
                switch (acaoMenu)
                {
                    case 0: pGG->fecharWindow(); break;
                    case 10: proximaFase = new Fases::FaseUm(this, false); break;
                    case 11: proximaFase = new Fases::FaseUm(this, true); break;
                    case 20: proximaFase = new Fases::FaseDois(this, false); break;
                    case 21: proximaFase = new Fases::FaseDois(this, true); break;
                    case 30: 
                        if (carregarSave()) {
                            estadoAtual = EstadoJogo::Jogando;
                            if (pFaseAtual) pGE->setOuvinte(pFaseAtual);
                        } else {
                            estadoAtual = EstadoJogo::NoMenu;
                        }
                        break;
                }

                // Transição de estado: Menu -> Fase
                if (proximaFase != nullptr)
                {
                    if (pFaseAtual) delete pFaseAtual;
                    pFaseAtual = proximaFase;
                    pFaseAtual->inicializar();
                    estadoAtual = EstadoJogo::Jogando;
                    pGE->setOuvinte(pFaseAtual); 
                }
            }
            break;
        
            // --- Estado: Gameplay ---
            case EstadoJogo::Jogando:
            {
                processarEventosJogando();
                
                // Verifica se pausou durante o processamento de eventos
                if (estadoAtual != EstadoJogo::Jogando) break;

                if (pFaseAtual) pFaseAtual->executar(delta);

                // Autosave periódico (2 segundos)
                if (saveClock.getElapsedTime().asSeconds() > 2.0f && pFaseAtual) {
                    pFaseAtual->salvar();
                    saveClock.restart();
                }

                // Verifica conclusão de fase, salva estado final e retorna ao menu
                if (pFaseAtual && pFaseAtual->getFaseConcluida())
                {
                    std::string snapshotFinal = pFaseAtual->salvarParaString();
                    std::ofstream ofs("save_state.txt", std::ios::trunc);
                    if (ofs.is_open()) { ofs << snapshotFinal; ofs.close(); }

                    estadoAtual = EstadoJogo::NoMenu;
                    pGE->setOuvinte(&menu); 
                    menu.resetInput();
                    delete pFaseAtual;
                    pFaseAtual = nullptr;
                }
                
                renderizar(); 
            }
            break;
            
            // --- Estado: Pause ---
            case EstadoJogo::Pausado:
            {
                // Renderiza o jogo congelado + Overlay escuro
                renderizar();
                sf::RectangleShape overlay;
                sf::Vector2u ws = pGG->getWindow().getSize();
                overlay.setSize(sf::Vector2f((float)ws.x, (float)ws.y));
                overlay.setFillColor(sf::Color(0, 0, 0, 160));
                pGG->desenhar(overlay);

                // Lógica de UI do Pause (Texto e Opções)
                if (pauseFont.getInfo().family.empty()) {
                    if (!pauseFont.openFromFile("font/PressStart2P-Regular.ttf")) {
                        std::cerr << "Aviso: nao conseguiu carregar a fonte de pause" << std::endl;
                    }
                }

                float centerX = ws.x / 2.f;
                float centerY = ws.y / 2.f;

                unsigned int titleSize = static_cast<unsigned int>(ws.y * 0.05f);
                sf::Text titulo(pauseFont, "PAUSADO", titleSize);
                titulo.setFillColor(sf::Color::White);
                sf::FloatRect bt = titulo.getLocalBounds();
                titulo.setOrigin({ bt.size.x / 2.f, bt.size.y / 2.f });
                titulo.setPosition({ centerX, centerY - 80.f });
                pGG->desenhar(titulo);

                std::vector<std::string> options = {"Continuar", "Voltar para o Menu"};
                std::vector<sf::FloatRect> optionBounds;
                
                // Renderiza opções do menu de pause
                for (size_t i = 0; i < options.size(); ++i) {
                    unsigned int optSize = static_cast<unsigned int>(ws.y * 0.03f);
                    sf::Text t(pauseFont, options[i], optSize);
                    t.setFillColor((int)i == pausePos ? sf::Color::Yellow : sf::Color::White);
                    sf::FloatRect b = t.getLocalBounds();
                    t.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
                    t.setPosition({ centerX, centerY - 10.f + (float)i * 50.f });
                    pGG->desenhar(t);
                    optionBounds.push_back(t.getGlobalBounds());
                }

                // Input Handling exclusivo do Pause (SFML 3 pollEvent)
                while (auto optEvent = pGG->pollEvent()) {
                    const auto &event = *optEvent;
                    if (event.is<sf::Event::Closed>()) pGG->fecharWindow();

                    // Interação Mouse
                    if (event.is<sf::Event::MouseMoved>()) {
                        sf::Vector2i mousePixel = sf::Mouse::getPosition(pGG->getWindow());
                        sf::Vector2f mouseCoord = pGG->mapPixelToCoords(mousePixel);
                        for (size_t i = 0; i < optionBounds.size(); ++i) {
                            if (optionBounds[i].contains(mouseCoord)) {
                                if (pausePos != (int)i) pausePos = (int)i;
                                break;
                            }
                        }
                    }

                    // Clique do Mouse
                    if (event.is<sf::Event::MouseButtonPressed>()) {
                        const auto *mb = event.getIf<sf::Event::MouseButtonPressed>();
                        if (mb && mb->button == sf::Mouse::Button::Left) {
                            sf::Vector2i mousePixel = sf::Mouse::getPosition(pGG->getWindow());
                            sf::Vector2f mouseCoord = pGG->mapPixelToCoords(mousePixel);
                            for (size_t i = 0; i < optionBounds.size(); ++i) {
                                if (optionBounds[i].contains(mouseCoord)) {
                                    pausePos = (int)i;
                                    // Simula Enter
                                    if (pausePos == 0) {
                                        estadoAtual = EstadoJogo::Jogando;
                                        pauseActive = false;
                                    } else if (pausePos == 1) {
                                        if (pFaseAtual) { delete pFaseAtual; pFaseAtual = nullptr; }
                                        estadoAtual = EstadoJogo::NoMenu;
                                        menu.resetInput();
                                    }
                                    break;
                                }
                            }
                        }
                    }

                    // Navegação Teclado
                    if (event.is<sf::Event::KeyPressed>()) {
                        const auto *kp = event.getIf<sf::Event::KeyPressed>();
                        if (!kp) continue;
                        switch (kp->code) {
                            case sf::Keyboard::Key::Up:
                            case sf::Keyboard::Key::W:
                                if (pausePos > 0) pausePos--; break;
                            case sf::Keyboard::Key::Down:
                            case sf::Keyboard::Key::S:
                                if (pausePos < (int)optionBounds.size() - 1) pausePos++; break;
                            case sf::Keyboard::Key::Escape: // Resume
                                estadoAtual = EstadoJogo::Jogando;
                                pauseActive = false;
                                break;
                            case sf::Keyboard::Key::Enter:
                                if (pausePos == 0) { // Continue
                                    estadoAtual = EstadoJogo::Jogando;
                                    pauseActive = false;
                                } else if (pausePos == 1) { // Exit to Menu
                                    if (pFaseAtual) { delete pFaseAtual; pFaseAtual = nullptr; }
                                    estadoAtual = EstadoJogo::NoMenu;
                                    menu.resetInput();
                                }
                                break;
                            default: break;
                        }
                    }
                }
            }
            break;   
        }
        pGG->exibir(); 
    }
}

// Loop de eventos durante gameplay (inclui trigger de Pause)
void Jogo::processarEventosJogando()
{
   while (auto optEvent = pGG->pollEvent())
    {
        const auto& event = *optEvent;

        if (event.is<sf::Event::Closed>()) pGG->fecharWindow();

        // Tecla ESC salva snapshot em memória e muda estado para Pausado
        if (event.is<sf::Event::KeyPressed>()) {
            const auto *kp = event.getIf<sf::Event::KeyPressed>();
                if (kp && kp->code == sf::Keyboard::Key::Escape) {
                    if (estadoAtual == EstadoJogo::Jogando && pFaseAtual) {
                        pauseSnapshot = pFaseAtual->salvarParaString();
                        pauseActive = true;
                        pausePos = 0;
                        pauseEnterDebounce = true;
                        estadoAtual = EstadoJogo::Pausado;
                        return; 
                    }
                }
        }

        if (pFaseAtual) pFaseAtual->tratarEvento(event);
    }
}

void Jogo::renderizar()
{
    if (pFaseAtual) pFaseAtual->desenhar();
}

void Jogo::adicionarAoRanking(int faseNum, std::string nome, int pontuacao)
{
    std::vector<RankingEntry>* pRanking = (faseNum == 1) ? &rankingFase1 : (faseNum == 2) ? &rankingFase2 : nullptr;
    if (!pRanking) return;

    pRanking->push_back({nome, pontuacao});

    // Ordena decrescente e limita ao Top 10
    std::sort(pRanking->begin(), pRanking->end(), std::greater<RankingEntry>());
    if (pRanking->size() > 10) pRanking->resize(10);

    salvar();
}

void Jogo::salvarDataBuffer(std::ostream& os)
{
    os << "RankingFase1" << std::endl;
    for (const auto& e : rankingFase1) os << e.nome << " " << e.pontuacao << std::endl;
    
    os << "---" << std::endl;
    
    os << "RankingFase2" << std::endl;
    for (const auto& e : rankingFase2) os << e.nome << " " << e.pontuacao << std::endl;
    
    os << "---" << std::endl;
}

void Jogo::salvar()
{
    std::ofstream ofs("ranking.txt", std::ios::trunc);
    if (!ofs.is_open()) return;
    salvarDataBuffer(ofs);
    ofs.close();
}

const std::vector<RankingEntry>& Jogo::getRankingFase1() const { return rankingFase1; }
const std::vector<RankingEntry>& Jogo::getRankingFase2() const { return rankingFase2; }