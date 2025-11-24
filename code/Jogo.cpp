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

Jogo::Jogo() :
    pGG(nullptr),
    pGE(nullptr),
    menu(),
    pFaseAtual(nullptr),
    estadoAtual(EstadoJogo::NoMenu) 
{
    inicializar();
}

Jogo::~Jogo()
{
    delete GerenciadorGrafico::getInstance();
    if(pFaseAtual) delete pFaseAtual;
}

void Jogo::inicializar()
{
    pGG = GerenciadorGrafico::getInstance();
    pGE = Gerenciadores::GerenciadorEventos::getInstance();
    
    Ente::setGerenciadorGrafico(pGG);

    // Define o Menu como o primeiro ouvinte
    pGE->setOuvinte(&menu);
    // Do not auto-load save on startup anymore; Load must be explicitly
    // requested through the menu. This avoids unexpected continuation after
    // recompiles/runs.
}

bool Jogo::carregarSave()
{
    std::ifstream ifs("save_state.txt");
    if (!ifs.is_open()) return false;

    std::string line;
    int faseNum = -1;
    bool modoDois = false;

    // Lê cabeçalho
    if (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string key;
        if (iss >> key >> faseNum) {
            // parsed
        }
    }
    if (std::getline(ifs, line)) {
        std::istringstream iss(line);
        std::string key; int v;
        if (iss >> key >> v) {
            if (key == "modoDoisJogadores") modoDois = (v != 0);
        }
    }

    // Cria a fase correta
    Fase* proxima = nullptr;
    if (faseNum == 1) proxima = new Fases::FaseUm(this, modoDois);
    else if (faseNum == 2) proxima = new Fases::FaseDois(this, modoDois);
    else return false;

    if (pFaseAtual) delete pFaseAtual;
    pFaseAtual = proxima;
    pFaseAtual->inicializar();

    // Lê blocos de entidade separados por '---'
    std::vector<std::string> block;
    while (std::getline(ifs, line)) {
        if (line == "---") {
            // process block
            std::string type;
            std::map<std::string, std::string> kv;
            for (auto &l : block) {
                std::istringstream iss(l);
                std::string a, b;
                if (!(iss >> a)) continue;
                if (!(iss >> b)) {
                    // line with single token might be the class name (e.g., "Jogador")
                    type = a;
                    continue;
                }
                kv[a] = b;
            }

            if (type == "Jogador") {
                int id = 1;
                if (kv.find("idJogador") != kv.end()) id = std::stoi(kv["idJogador"]);
                Entidades::Personagens::Jogador* pj = (id == 2) ? pFaseAtual->getJogador2() : pFaseAtual->getJogador1();
                if (pj) {
                    // Prefer the exact sprite position if present (saved by Personagem::salvarDataBuffer)
                    if (kv.find("spritePosX") != kv.end() && kv.find("spritePosY") != kv.end()) {
                        float x = std::stof(kv["spritePosX"]);
                        float y = std::stof(kv["spritePosY"]);
                        pj->setPosition({x, y});
                    }
                    else if (kv.find("posX") != kv.end() && kv.find("posY") != kv.end()) {
                        float x = std::stof(kv["posX"]);
                        float y = std::stof(kv["posY"]);
                        pj->setPosition({x, y});
                    }
                    if (kv.find("pontos") != kv.end()) {
                        pj->setPontos(std::stof(kv["pontos"]));
                    }
                    if (kv.find("vidas") != kv.end()) {
                        pj->setVidas(std::stoi(kv["vidas"]));
                    }
                }
            }

            block.clear();
        }
        else {
            if (!line.empty()) block.push_back(line);
        }
    }

    return true;
}

void Jogo::executar()
{
    while (pGG->isWindowOpen())
    {
        pGG->limpar(sf::Color::Black);

        float delta = relogio.restart().asSeconds();

        switch (estadoAtual)
        {
            case EstadoJogo::NoMenu:
            {
                // Let the event manager forward events to the menu listener
                pGE->setOuvinte(&menu);
                // Process accumulated events now (only for menu)
                pGE->processarEventos();
                int acaoMenu = menu.executar(rankingFase1, rankingFase2);
                Fases::Fase* proximaFase = nullptr;

                switch (acaoMenu)
                {
                    case 0:
                        pGG->fecharWindow();
                        break;
                    case 10:
                        proximaFase = new Fases::FaseUm(this, false);
                        break;
                    case 11:
                        proximaFase = new Fases::FaseUm(this, true);
                        break;
                    case 20:
                        proximaFase = new Fases::FaseDois(this, false);
                        break;
                    case 21:
                        proximaFase = new Fases::FaseDois(this, true);
                        break;
                       case 30: // Load
                           if (carregarSave()) {
                               estadoAtual = EstadoJogo::Jogando;
                           } else {
                               // Falha ao carregar; permanece no menu
                               estadoAtual = EstadoJogo::NoMenu;
                           }
                           break;
                }

                // Se uma fase foi escolhida, transiciona o estado
                if (proximaFase != nullptr)
                {
                    if (pFaseAtual) { delete pFaseAtual; } // Limpa a fase antiga
                    pFaseAtual = proximaFase;
                    pFaseAtual->inicializar(); // Inicializa a nova
                    estadoAtual = EstadoJogo::Jogando;
                    pGE->setOuvinte(pFaseAtual); // <--- TROCA O OUVINTE PARA A FASE
                }
            }
            break;
        
            case EstadoJogo::Jogando:
            {
                processarEventosJogando();
                // Se o estado foi alterado para Pausado dentro do handler de eventos,
                // interrompe a execução restante deste case para evitar que saves
                // e updates adicionais ocorram no mesmo frame.
                if (estadoAtual != EstadoJogo::Jogando) break;

                if (pFaseAtual)
                {
                    pFaseAtual->executar(delta);
                }

                // Autosave periódico a cada 2 segundos
                if (saveClock.getElapsedTime().asSeconds() > 2.0f && pFaseAtual) {
                    pFaseAtual->salvar();
                    saveClock.restart();
                }

                if (pFaseAtual && pFaseAtual->getFaseConcluida())
                {
                    estadoAtual = EstadoJogo::NoMenu;
                    pGE->setOuvinte(&menu); // <--- TROCA O OUVINTE DE VOLTA PRO MENU
                    menu.resetInput();
                    
                    delete pFaseAtual;
                    pFaseAtual = nullptr;
                }
                
                renderizar(); 
            }
            break;
            
            case EstadoJogo::Pausado:
            {
                // Quando pausado, não executamos atualizações das entidades.
                // Apenas processamos eventos de menu de pausa e desenhamos o estado atual.
                // NÃO chamar `processarEventosJogando()` aqui pois ele consome todos os
                // eventos (incluindo teclas/mouse) antes do loop de pausa; em vez disso
                // usamos o loop local abaixo para tratar eventos de pausa.

                // Desenha a cena atual como pano de fundo
                renderizar();

                // Overlay semi-transparente
                sf::RectangleShape overlay;
                sf::Vector2u ws = pGG->getWindow().getSize();
                overlay.setSize(sf::Vector2f((float)ws.x, (float)ws.y));
                overlay.setFillColor(sf::Color(0, 0, 0, 160));
                pGG->desenhar(overlay);

                // Carrega fonte de pause se necessário
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
                optionBounds.reserve(options.size());
                for (size_t i = 0; i < options.size(); ++i) {
                    unsigned int optSize = static_cast<unsigned int>(ws.y * 0.03f);
                    sf::Text t(pauseFont, options[i], optSize);
                    t.setFillColor((int)i == pausePos ? sf::Color::Yellow : sf::Color::White);
                    sf::FloatRect b = t.getLocalBounds();
                    t.setOrigin({ b.size.x / 2.f, b.size.y / 2.f });
                    t.setPosition({ centerX, centerY - 10.f + (float)i * 50.f });
                    pGG->desenhar(t);

                    // store global bounds for mouse interaction
                    optionBounds.push_back(t.getGlobalBounds());
                }

                // Poll pause-specific events (mouse + keyboard)
                while (auto optEvent = pGG->pollEvent()) {
                    const auto &event = *optEvent;
                    if (event.is<sf::Event::Closed>()) {
                        pGG->fecharWindow();
                    }

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

                    if (event.is<sf::Event::MouseButtonPressed>()) {
                        const auto *mb = event.getIf<sf::Event::MouseButtonPressed>();
                        if (mb && mb->button == sf::Mouse::Button::Left) {
                            sf::Vector2i mousePixel = sf::Mouse::getPosition(pGG->getWindow());
                            sf::Vector2f mouseCoord = pGG->mapPixelToCoords(mousePixel);
                            for (size_t i = 0; i < optionBounds.size(); ++i) {
                                if (optionBounds[i].contains(mouseCoord)) {
                                    pausePos = (int)i;
                                    // emulate Enter
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
                            case sf::Keyboard::Key::Escape:
                                // Resume
                                estadoAtual = EstadoJogo::Jogando;
                                pauseActive = false;
                                break;
                            case sf::Keyboard::Key::Enter:
                                if (pausePos == 0) {
                                    // Continue
                                    estadoAtual = EstadoJogo::Jogando;
                                    pauseActive = false;
                                } else if (pausePos == 1) {
                                    // Voltar para o menu: descarta a fase atual
                                    if (pFaseAtual) {
                                        delete pFaseAtual;
                                        pFaseAtual = nullptr;
                                    }
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

void Jogo::processarEventosJogando()
{
   while (auto optEvent = pGG->pollEvent())
    {
        const auto& event = *optEvent;

        if (event.is<sf::Event::Closed>())
        {
            pGG->fecharWindow();
        }

        // Pause toggle: abre menu de pausa com ESC
        if (event.is<sf::Event::KeyPressed>()) {
            const auto *kp = event.getIf<sf::Event::KeyPressed>();
                if (kp && kp->code == sf::Keyboard::Key::Escape) {
                    if (estadoAtual == EstadoJogo::Jogando && pFaseAtual) {
                        // Create an in-memory snapshot (fast) instead of writing a file
                        // to avoid blocking the main loop while pausing.
                        pauseSnapshot = pFaseAtual->salvarParaString();
                        // Optional: also append a small debug trace
                        {
                            std::ofstream dbg("debug.log", std::ios::app);
                            dbg << "[DEBUG] pause snapshot created, size=" << pauseSnapshot.size() << "\n";
                        }
                        pauseActive = true;
                        pausePos = 0;
                        pauseEnterDebounce = true;
                        estadoAtual = EstadoJogo::Pausado;
                        return; // stop processing further events for this frame
                    }
                }
        }

        if (pFaseAtual)
        {
            pFaseAtual->tratarEvento(event);
        }
    }
}

void Jogo::renderizar()
{
    if (pFaseAtual)
    {
        pFaseAtual->desenhar();
    }
}

void Jogo::adicionarAoRanking(int faseNum, std::string nome, int pontuacao)
{
    // Escolhe o vetor correto
    std::vector<RankingEntry>* pRanking = nullptr;
    if (faseNum == 1) {
        pRanking = &rankingFase1;
    } else if (faseNum == 2) {
        pRanking = &rankingFase2;
    }

    if (!pRanking) return; // Número de fase inválido

    pRanking->push_back({nome, pontuacao});

    // Ordena o vetor (do maior para o menor)
    std::sort(pRanking->begin(), pRanking->end(), std::greater<RankingEntry>());

    // Mantém apenas os Top 10
    if (pRanking->size() > 10)
    {
        pRanking->resize(10);
    }
}

const std::vector<RankingEntry>& Jogo::getRankingFase1() const
{
    return rankingFase1;
}

const std::vector<RankingEntry>& Jogo::getRankingFase2() const
{
    return rankingFase2;
}
