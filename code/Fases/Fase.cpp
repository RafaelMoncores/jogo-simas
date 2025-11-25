#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Jogo.hpp"
#include "../Entidades/BolaDeFogo.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include "../Entidades/Personagens/Vampiro.hpp"
#include "../Entidades/Personagens/Gosma.hpp"
#include "../Entidades/Personagens/Dragao.hpp"
#include "../Entidades/Obstaculos/Obstaculo.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Entidades/Obstaculos/Trampolim.hpp"
#include <iostream>
#include <string>
#include <cstdint>
#include <cctype>
#include <fstream>
#include <sstream>

namespace Fases
{
    Fase::Fase(Jogo* pJ, int num, bool doisJogadores) :
        Ente(),
        pJogo(pJ),
        jogador1(nullptr),
        jogador2(nullptr),
        modoDoisJogadores(doisJogadores),
        faseConcluida(false),
        estadoFim(EstadoFim::MostrandoOpcoes),
        posJogadorInicial(0.f, 0.f),
        posJogador2Inicial(80.f, 0.f),
        pontuacaoFinalCache(-1),
        posBotaoFim(0),
        navPressionado(true),
        iniciais(""),
        faseNum(num),
        numTentativas(1),
        enterPressionado(true),
        jogadorQueFinalizar(0)
    {
        inicializarUI();
    }

    Fase::~Fase()
    {
        listaEntidades.limpar();
        gerenciadorColisoes.limpar();
        
        jogador1 = nullptr;
        jogador2 = nullptr;
    }

    // Inicializa elementos de UI: textos de vidas, pontos, botões e input
    void Fase::inicializarUI()
    {
        const float W_BASE = 1920.0f;
        const float H_BASE = 1080.0f;
        const unsigned int FONT_SIZE_UI = static_cast<unsigned int>(H_BASE * 0.022f);
        const unsigned int FONT_SIZE_FINAL = static_cast<unsigned int>(H_BASE * 0.045f);
        const unsigned int FONT_SIZE_BUTTON = static_cast<unsigned int>(H_BASE * 0.032f);

        if (!uiFont.openFromFile("font/PressStart2P-Regular.ttf"))
        {
            std::cerr << "ERRO: Nao foi possivel carregar a fonte 'PressStart2P-Regular.ttf'" << std::endl;
        }

        const float X_LEFT = 5.f;
        
        const float X_CENTER_LEFT = W_BASE * 0.350f;
        
        const float X_RIGHT = W_BASE * 0.963f;

        vidasTextP1.emplace(uiFont);
        vidasTextP1->setCharacterSize(FONT_SIZE_UI);
        vidasTextP1->setFillColor(sf::Color::White);
        vidasTextP1->setPosition({X_LEFT, 10.f}); 
        vidasTextP1->setString("P1 Vidas: ");

        pontosTextP1.emplace(uiFont);
        pontosTextP1->setCharacterSize(FONT_SIZE_UI);
        pontosTextP1->setFillColor(sf::Color::White);
        pontosTextP1->setString("P1 Pontos: "); 
        sf::FloatRect boundsPontos = pontosTextP1->getLocalBounds();
        pontosTextP1->setOrigin({boundsPontos.size.x / 2.f, 0.f});
        pontosTextP1->setPosition({X_CENTER_LEFT, 10.f});

        vidasTextP2.emplace(uiFont);
        vidasTextP2->setCharacterSize(FONT_SIZE_UI);
        vidasTextP2->setFillColor(sf::Color::White);
        vidasTextP2->setPosition({X_LEFT, 50.f});
        vidasTextP2->setString("P2 Vidas: ");

        pontosTextP2.emplace(uiFont);
        pontosTextP2->setCharacterSize(FONT_SIZE_UI);
        pontosTextP2->setFillColor(sf::Color::White);
        pontosTextP2->setString("P2 Pontos: "); 
        sf::FloatRect boundsPontosP2 = pontosTextP2->getLocalBounds();
        pontosTextP2->setOrigin({boundsPontosP2.size.x / 2.f, 0.f});
        pontosTextP2->setPosition({X_CENTER_LEFT, 50.f});

        tentativasText.emplace(uiFont); 
        tentativasText->setCharacterSize(FONT_SIZE_UI);
        tentativasText->setFillColor(sf::Color::White);
        tentativasText->setString("tentativas: "); 
        sf::FloatRect bounds = tentativasText->getLocalBounds();
        tentativasText->setOrigin({bounds.size.x, 0.f});
        tentativasText->setPosition({X_RIGHT, 10.f});

        pontuacaoFinalText.emplace(uiFont); 
        pontuacaoFinalText->setCharacterSize(FONT_SIZE_FINAL);
        pontuacaoFinalText->setFillColor(sf::Color::White);
        pontuacaoFinalText->setOrigin({pontuacaoFinalText->getLocalBounds().size.x / 2.f,pontuacaoFinalText->getLocalBounds().size.y / 2.f});
        pontuacaoFinalText->setPosition({W_BASE / 2.f, H_BASE / 2.f - 50.f}); 
        pontuacaoFinalText->setString("Pontuacao: ");

        // Botão Salvar (opção exibida no fim da fase)
        botaoSalvarText.emplace(uiFont);
        botaoSalvarText->setCharacterSize(FONT_SIZE_BUTTON);
        botaoSalvarText->setString("Salvar no Ranking");
        sf::FloatRect boundsSalvar = botaoSalvarText->getLocalBounds();
        botaoSalvarText->setOrigin({boundsSalvar.size.x / 2.f, boundsSalvar.size.y / 2.f});
        botaoSalvarText->setPosition({W_BASE / 2.f, (H_BASE / 2.f) + 10.f}); 

        // Botão Menu (leva de volta ao menu principal)
        botaoMenuText.emplace(uiFont);
        botaoMenuText->setCharacterSize(FONT_SIZE_BUTTON);
        botaoMenuText->setString("Menu");
        sf::FloatRect boundsMenu = botaoMenuText->getLocalBounds();
        botaoMenuText->setOrigin({boundsMenu.size.x / 2.f, boundsMenu.size.y / 2.f});
        botaoMenuText->setPosition({W_BASE / 2.f, (H_BASE / 2.f) + 70.f}); 

        // Texto de Input para iniciais do ranking
        inputIniciaisText.emplace(uiFont);
        inputIniciaisText->setCharacterSize(FONT_SIZE_BUTTON);
        inputIniciaisText->setString("Iniciais (3): ___");
        sf::FloatRect boundsInput = inputIniciaisText->getLocalBounds();
        inputIniciaisText->setOrigin({boundsInput.size.x / 2.f, boundsInput.size.y / 2.f});
        inputIniciaisText->setPosition({W_BASE / 2.f, (H_BASE / 2.f) + 10.f});
    }
    
    void Fase::inicializar()
    {
        listaEntidades.limpar();
        gerenciadorColisoes.limpar();
        
        jogador1 = nullptr;
        jogador2 = nullptr;

        Gerenciadores::GerenciadorGrafico::getInstance()->setViewBounds(0.f, 0.f, 1920.f, 1080.f);

        posJogador2Inicial = posJogadorInicial + sf::Vector2f(80.f, 0.f);

        jogador1 = new Entidades::Personagens::Jogador(posJogadorInicial, 1);
        jogador1->setPosition(posJogadorInicial);
        listaEntidades.incluir(jogador1); 

        if (modoDoisJogadores)
        {
            jogador2 = new Entidades::Personagens::Jogador(posJogador2Inicial, 2);
            jogador2->setPosition(posJogador2Inicial);
            listaEntidades.incluir(jogador2);
        }

        criarObstaculos();
        criarInimigos();
    }

    void Fase::inicializarParaLoad()
    {
        listaEntidades.limpar();
        gerenciadorColisoes.limpar();
        
        jogador1 = nullptr;
        jogador2 = nullptr;

        Gerenciadores::GerenciadorGrafico::getInstance()->setViewBounds(0.f, 0.f, 1920.f, 1080.f);

        posJogador2Inicial = posJogadorInicial + sf::Vector2f(80.f, 0.f);

        jogador1 = new Entidades::Personagens::Jogador(posJogadorInicial, 1);
        jogador1->setPosition(posJogadorInicial);
        listaEntidades.incluir(jogador1); 

        if (modoDoisJogadores)
        {
            jogador2 = new Entidades::Personagens::Jogador(posJogador2Inicial, 2);
            jogador2->setPosition(posJogador2Inicial);
            listaEntidades.incluir(jogador2);
        }

        // NOTE: NÃO chamar criarObstaculos() - plataformas serão restauradas do snapshot
        // NOTA: Também não chamar criarInimigos() - serão restaurados do snapshot
    }

    // Encaminha eventos para o estado correto: entrada de iniciais quando pedindo
    // ou ignora em outros momentos.
    void Fase::processarEvento(const sf::Event& evento)
    {
        bool p1Completo = (jogador1 && jogador1->getCompletouFase());
        bool p2Completo = (modoDoisJogadores && jogador2 && jogador2->getCompletouFase());

        if ((p1Completo || p2Completo) && estadoFim == EstadoFim::PedindoIniciais)
        {
            processarInputIniciais(evento);
        }
    }

    // Trata entrada de texto para as iniciais do ranking (TextEntered + backspace)
    void Fase::processarInputIniciais(const sf::Event& evento)
    {
        if (evento.is<sf::Event::TextEntered>())
        {
            const auto* te = evento.getIf<sf::Event::TextEntered>();
            if (!te) return;

            std::uint32_t valorUnicode = te->unicode;

            // Backspace: apaga última letra
            if (valorUnicode == 8)
            {
                if (!iniciais.empty()) iniciais.pop_back();
            }
            // Caracter imprimível: adiciona se houver espaço (máx 3)
            else if (valorUnicode >= 32 && valorUnicode <= 126 && iniciais.length() < 3)
            {
                char c = static_cast<char>(valorUnicode);
                iniciais += std::toupper(c);
            }

            // Atualiza texto visual com placeholder
            if (inputIniciaisText)
            {
                std::string placeholder = "___";
                std::string textoAtual = iniciais;
                if (textoAtual.length() < 3) textoAtual += placeholder.substr(textoAtual.length());
                inputIniciaisText->setString("Iniciais (3): " + textoAtual);
            }
        }
        
        if (evento.is<sf::Event::KeyReleased>())
        {
             const auto* kr = evento.getIf<sf::Event::KeyReleased>();
             if (kr && kr->code == sf::Keyboard::Key::Enter)
             {
                 enterPressionado = false; 
             }
        }
    }
    
    void Fase::executar(float delta)
    {
        // --- 1. LÓGICA DE FASE CONGELADA (FIM DE JOGO) ---
        bool p1Completo = (jogador1 && jogador1->getCompletouFase());
        bool p2Completo = (modoDoisJogadores && jogador2 && jogador2->getCompletouFase());

        gerenciadorColisoes.limpar();
        gerenciadorColisoes.setJogador(jogador1);

        if (p1Completo || p2Completo)
        {
            if (pontuacaoFinalCache == -1) 
            {
                if (p1Completo) {
                    pontuacaoFinalCache = jogador1->getPontos();
                    jogadorQueFinalizar = 1;
                } else {
                    pontuacaoFinalCache = jogador2->getPontos();
                    jogadorQueFinalizar = 2;
                }
            
                if (pontuacaoFinalText)
                {
                    pontuacaoFinalText->setString("PONTUACAO: " + std::to_string(pontuacaoFinalCache));
                    
                    sf::FloatRect bounds = pontuacaoFinalText->getLocalBounds();
                    pontuacaoFinalText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
                }
            }

            switch (estadoFim)
            {
                case EstadoFim::MostrandoOpcoes:
                {
                    
                }
                break;

                case EstadoFim::PedindoIniciais:
                {
                    
                    
                }
                break;
            }
            
            return; 
        }

        // --- 2. LÓGICA DE JOGO NORMAL (COLISÕES E UPDATES) ---
        
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;

            // Tenta converter e adicionar no gerenciador
            if (auto* pInim = dynamic_cast<Entidades::Personagens::Inimigo*>(pE)) {
                gerenciadorColisoes.incluirInimigo(pInim);
            }
            else if (auto* pObst = dynamic_cast<Entidades::Obstaculos::Obstaculo*>(pE)) {
                gerenciadorColisoes.incluirObstaculo(pObst);
            }
            else if (auto* pFogo = dynamic_cast<Entidades::BolaDeFogo*>(pE)) {
                gerenciadorColisoes.incluirProjetil(pFogo);
            }
            
            // Executa o update individual da entidade
            pE->executar(delta);
        }

           // Executa colisões para o Jogador 1 (e reseta inimigos nesta primeira passada)
           gerenciadorColisoes.executar(true);

           // Se houver segundo jogador, executa novamente SEM resetar o estado dos inimigos,
           // para não sobrescrever flags (como 'podePular') já determinadas.
           if (modoDoisJogadores && jogador2) {
               gerenciadorColisoes.setJogador(jogador2);
               gerenciadorColisoes.executar(false);
               gerenciadorColisoes.setJogador(jogador1);
           }

        // --- 3. ATUALIZAÇÃO DA UI (VIDAS, PONTOS, TENTATIVAS) ---
        
        if (jogador1)
        {
            if (vidasTextP1) 
            {
                vidasTextP1->setString("P1 Vidas: " + std::to_string(jogador1->getVidas()));
            }
            if (pontosTextP1) 
            {
                pontosTextP1->setString("P1 Pontos: " + std::to_string(jogador1->getPontos()));
            }
        }
        else if (vidasTextP1)
        {
            vidasTextP1->setString("P1 Vidas: 0");
        }

        if (modoDoisJogadores)
        {
            if (jogador2)
            {
                if (vidasTextP2)
                {
                    vidasTextP2->setString("P2 Vidas: " + std::to_string(jogador2->getVidas()));
                }
                if (pontosTextP2)
                {
                    pontosTextP2->setString("P2 Pontos: " + std::to_string(jogador2->getPontos()));
                }
            }
            else if (vidasTextP2)
            {
                vidasTextP2->setString("P2 Vidas: 0");
            }
        }

        if (tentativasText)
        {
            tentativasText->setString("Tentativas: " + std::to_string(numTentativas));
            sf::FloatRect bounds = tentativasText->getLocalBounds();
            tentativasText->setOrigin({bounds.size.x, 0.f});

            sf::Vector2u winSize = Gerenciadores::GerenciadorGrafico::getInstance()->getWindow().getSize();
            float xPos = static_cast<float>(winSize.x) - 20.f;
            tentativasText->setPosition({xPos, 10.f});
        }

        // --- 4. LÓGICA DE MORTE E REINÍCIO DE FASE ---
        bool p1Morto = (jogador1 && jogador1->getVidas() <= 0);
        bool p2Morto = (modoDoisJogadores && jogador2 && jogador2->getVidas() <= 0);
        bool p2NaoJoga = !modoDoisJogadores;

        if ( (p1Morto && p2NaoJoga) || (p1Morto && p2Morto) )
        {
            std::cout << "Sem vidas! Reiniciando a fase..." << std::endl;
                
            numTentativas++; 
            
            int pontosIniciais = 1000 - ((numTentativas - 1) * 100);
            
            if (pontosIniciais < 0) pontosIniciais = 0; 
            
            inicializar(); 
            
            if (jogador1) 
            {
                jogador1->setPontos( (float)pontosIniciais );
            }
            if (jogador2) 
            {
                jogador2->setPontos( (float)pontosIniciais );
            }
        }

        // --- 5. LÓGICA DE LIMPEZA DE ENTIDADES (INIMIGOS MORTOS, PROJÉTEIS) ---
        bool removeu = true;
        while (removeu)
        {
            removeu = false;
            for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
            {
                Entidades::Entidade* pE = listaEntidades.getAtual();
                if (pE == NULL) break;

                bool deletar = false;

                if (Entidades::BolaDeFogo* pFogo = dynamic_cast<Entidades::BolaDeFogo*>(pE))
                {
                    if (!pFogo->getAtivo()) deletar = true;
                }
                else if (Entidades::Personagens::Inimigo* pInim = dynamic_cast<Entidades::Personagens::Inimigo*>(pE))
                {
                    if (pInim->getVidas() <= 0) deletar = true;
                }
                
                if (deletar)
                {
                    listaEntidades.remover(pE); 
                    delete pE;
                    removeu = true;             
                    break; 
                }
            }
        }
    }

    bool Fase::getFaseConcluida() const
    {
        return faseConcluida;
    }

    void Fase::desenhar()
    {
        Gerenciadores::GerenciadorGrafico* pGG = Gerenciadores::GerenciadorGrafico::getInstance();

        pGG->aplicarView();
        if (bgFase) pGG->desenhar(*bgFase);

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->desenhar();
        }
        
        pGG->resetarView();
        if (jogador1 && jogador1->getCompletouFase())
        {
            // Desenha a pontuação final (sempre)
            if (pontuacaoFinalText) pGG->desenhar(*pontuacaoFinalText);

            switch (estadoFim)
            {
                case EstadoFim::MostrandoOpcoes:
                {
                            // Reposiciona os elementos do menu final com base no tamanho real da janela
                            sf::Vector2u winSize = pGG->getWindow().getSize();
                            float centerX = static_cast<float>(winSize.x) / 2.f;
                            float centerY = static_cast<float>(winSize.y) / 2.f;

                            // Recentra a pontuação final
                            if (pontuacaoFinalText)
                            {
                                sf::FloatRect bPts = pontuacaoFinalText->getLocalBounds();
                                pontuacaoFinalText->setOrigin({bPts.size.x / 2.f, bPts.size.y / 2.f});
                                pontuacaoFinalText->setPosition({centerX, centerY - 50.f});
                            }

                            // Lógica de Destaque para "Salvar"
                            if (botaoSalvarText)
                            {
                                sf::FloatRect boundsSalvar = botaoSalvarText->getLocalBounds();
                                botaoSalvarText->setOrigin({boundsSalvar.size.x / 2.f, boundsSalvar.size.y / 2.f});
                                botaoSalvarText->setPosition({centerX, centerY + 10.f});

                                if (posBotaoFim == 0) { // Se "Salvar" está selecionado
                                    botaoSalvarText->setOutlineThickness(4.f);
                                    botaoSalvarText->setFillColor(sf::Color::White);
                                } else {
                                    botaoSalvarText->setOutlineThickness(0.f);
                                    botaoSalvarText->setFillColor(sf::Color::White);
                                }
                                pGG->desenhar(*botaoSalvarText);
                            }
                    
                    // Lógica de Destaque para "Menu"
                    if (botaoMenuText)
                    {
                        sf::FloatRect boundsMenu = botaoMenuText->getLocalBounds();
                        botaoMenuText->setOrigin({boundsMenu.size.x / 2.f, boundsMenu.size.y / 2.f});
                        botaoMenuText->setPosition({centerX, centerY + 70.f});

                        if (posBotaoFim == 1) { // Se "Menu" está selecionado
                            botaoMenuText->setOutlineThickness(4.f);
                            botaoMenuText->setFillColor(sf::Color::White);
                        } else {
                            botaoMenuText->setOutlineThickness(0.f);
                            botaoMenuText->setFillColor(sf::Color::White);
                        }
                        pGG->desenhar(*botaoMenuText);
                    }
                    break;
                }
                
                case EstadoFim::PedindoIniciais:
                    if (inputIniciaisText) pGG->desenhar(*inputIniciaisText);
                    break;
            }
        }
        else
        {
            if (vidasTextP1) pGG->desenhar(*vidasTextP1);
            if (pontosTextP1) pGG->desenhar(*pontosTextP1);
            
            if (modoDoisJogadores)
            {
                if (vidasTextP2) pGG->desenhar(*vidasTextP2);
                if (pontosTextP2) pGG->desenhar(*pontosTextP2);
            }

            if (tentativasText) pGG->desenhar(*tentativasText);
        }
    }

    sf::FloatRect Fase::getBoundingBox() const
    {
        return sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}); 
    }

    void Fase::salvar()
    {
        std::ofstream ofs("save_state.txt", std::ios::trunc);
        if (!ofs.is_open()) {
            std::cerr << "Erro ao abrir arquivo de salvamento" << std::endl;
            return;
        }

        ofs << "Fase " << faseNum << std::endl;
            ofs << "modoDoisJogadores " << (modoDoisJogadores ? 1 : 0) << std::endl;

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;

            // Fornece o stream e chama salvar() que por sua vez chama salvarDataBuffer()
            pE->setOutputStreamPublic(&ofs);
            pE->salvar();
            // Evita deixar o ponteiro para o stream local após o fechamento
            pE->setOutputStreamPublic(nullptr);

            ofs << "---" << std::endl;
        }

        ofs.close();
    }

    void Fase::salvarComNome(const std::string& caminho)
    {
        // Delegar para o buffer-string e então escrever no arquivo (reusa lógica)
        std::string data = salvarParaString();
        std::ofstream ofs(caminho, std::ios::trunc);
        if (!ofs.is_open()) {
            std::cerr << "Erro ao abrir arquivo de salvamento: " << caminho << std::endl;
            return;
        }
        ofs << data;
        ofs.close();
    }

    std::string Fase::salvarParaString()
    {
        std::ostringstream oss;
        // Opcional: grava um cabeçalho com identificação da fase e informações úteis
        oss << "Fase " << faseNum << std::endl;
        oss << "modoDoisJogadores " << (modoDoisJogadores ? 1 : 0) << std::endl;

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;

            pE->setOutputStreamPublic(&oss);
            pE->salvar();
            // Evita deixar o ponteiro para o stream local após o fechamento
            pE->setOutputStreamPublic(nullptr);

            oss << "---" << std::endl;
        }

        return oss.str();
    }

    // Remove projeteis e inimigos da lista (mantem jogadores e obstaculos)
    void Fase::limparEntidadesDinamicas()
    {
        bool removeu = true;
        while (removeu)
        {
            removeu = false;
            for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
            {
                Entidades::Entidade* pE = listaEntidades.getAtual();
                if (pE == NULL) break;

                bool deletar = false;

                if (Entidades::BolaDeFogo* pFogo = dynamic_cast<Entidades::BolaDeFogo*>(pE))
                {
                    deletar = true;
                }
                else if (Entidades::Personagens::Inimigo* pInim = dynamic_cast<Entidades::Personagens::Inimigo*>(pE))
                {
                    deletar = true;
                }

                if (deletar)
                {
                    listaEntidades.remover(pE);
                    delete pE;
                    removeu = true;
                    break;
                }
            }
        }
    }

    void Fase::restaurarPlataforma(const std::map<std::string, std::string>& kv)
    {
        using namespace Entidades;

        float x = 0.f, y = 0.f, w = 100.f, h = 20.f;
        if (kv.find("posX") != kv.end()) x = std::stof(kv.at("posX"));
        if (kv.find("posY") != kv.end()) y = std::stof(kv.at("posY"));
        if (kv.find("sizeX") != kv.end()) w = std::stof(kv.at("sizeX"));
        if (kv.find("sizeY") != kv.end()) h = std::stof(kv.at("sizeY"));

        // Usa a textura salva, ou padrão se não houver
        std::string caminhoTextura = "tileSets/fase1/plataforma.png";
        if (kv.find("textura") != kv.end()) {
            caminhoTextura = kv.at("textura");
        }

        Obstaculos::Plataforma* pPlat = new Obstaculos::Plataforma(
            {x, y}, 
            {w, h}, 
            caminhoTextura
        );
        
        pPlat->setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico::getInstance());
        listaEntidades.incluir(static_cast<Entidade*>(pPlat));
    }

    void Fase::restaurarTrampolim(const std::map<std::string, std::string>& kv)
    {
        using namespace Entidades;

        float x = 0.f, y = 0.f, w = 50.f, h = 50.f;
        if (kv.find("posX") != kv.end()) x = std::stof(kv.at("posX"));
        if (kv.find("posY") != kv.end()) y = std::stof(kv.at("posY"));
        if (kv.find("sizeX") != kv.end()) w = std::stof(kv.at("sizeX"));
        if (kv.find("sizeY") != kv.end()) h = std::stof(kv.at("sizeY"));

        float forcaRebote = 600.f;
        if (kv.find("forcaRebote") != kv.end()) {
            forcaRebote = std::stof(kv.at("forcaRebote"));
        }

        std::string caminhoTextura = "tileSets/trampolim.png";
        if (kv.find("textura") != kv.end()) {
            caminhoTextura = kv.at("textura");
        }

        Obstaculos::Trampolim* pTramp = new Obstaculos::Trampolim(
            {x, y},
            {w, h},
            caminhoTextura,
            forcaRebote
        );
        
        pTramp->setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico::getInstance());
        listaEntidades.incluir(static_cast<Entidade*>(pTramp));
    }

    void Fase::restaurarBolaDeFogo(const std::map<std::string, std::string>& kv)
    {
        using namespace Entidades;

        float x = 0.f, y = 0.f;
        if (kv.find("posX") != kv.end()) x = std::stof(kv.at("posX"));
        if (kv.find("posY") != kv.end()) y = std::stof(kv.at("posY"));

        sf::Vector2f pos{x, y};
        sf::Vector2f dir{1.f, 0.f};
        if (kv.find("dirX") != kv.end()) dir.x = std::stof(kv.at("dirX"));
        if (kv.find("dirY") != kv.end()) dir.y = std::stof(kv.at("dirY"));

        float velProj = 600.f;
        if (kv.find("velProjetil") != kv.end()) velProj = std::stof(kv.at("velProjetil"));

        bool pertence = false;
        if (kv.find("pertenceAoJogador") != kv.end()) pertence = (kv.at("pertenceAoJogador") != "0");

        int owner = 0;
        if (kv.find("ownerId") != kv.end()) owner = std::stoi(kv.at("ownerId"));

        BolaDeFogo* pF = new BolaDeFogo(pos, dir, velProj, pertence, owner);
        if (kv.find("ativo") != kv.end()) pF->setAtivo(kv.at("ativo") != "0");
        pF->setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico::getInstance());
        listaEntidades.incluir(static_cast<Entidade*>(pF));
    }

    void Fase::restaurarVampiro(const std::map<std::string, std::string>& kv)
    {
        using namespace Entidades;

        float x = 0.f, y = 0.f;
        if (kv.find("posX") != kv.end()) x = std::stof(kv.at("posX"));
        if (kv.find("posY") != kv.end()) y = std::stof(kv.at("posY"));

        float tam = 1.0f;
        if (kv.find("tamanho") != kv.end()) tam = std::stof(kv.at("tamanho"));

        Personagens::Vampiro* pV = new Personagens::Vampiro({x, y}, tam, jogador1, jogador2);

        if (kv.find("tempoTotal") != kv.end()) {
            pV->setTempoTotal(std::stof(kv.at("tempoTotal")));
        }
        if (kv.find("direcao") != kv.end()) {
            pV->setDirecao(std::stoi(kv.at("direcao")));
        }
        if (kv.find("vidas") != kv.end()) pV->setVidas(std::stoi(kv.at("vidas")));
        if (kv.find("spritePosX") != kv.end() && kv.find("spritePosY") != kv.end()) {
            float sx = std::stof(kv.at("spritePosX"));
            float sy = std::stof(kv.at("spritePosY"));
            pV->setPosition({sx, sy});
        }

        pV->setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico::getInstance());
        listaEntidades.incluir(static_cast<Entidade*>(pV));
    }

    void Fase::restaurarGosma(const std::map<std::string, std::string>& kv)
    {
        using namespace Entidades;

        float x = 0.f, y = 0.f;
        if (kv.find("posX") != kv.end()) x = std::stof(kv.at("posX"));
        if (kv.find("posY") != kv.end()) y = std::stof(kv.at("posY"));

        // Cria uma gosma sem plataforma (pode estar no ar)
        Personagens::Gosma* pG = new Personagens::Gosma({x, y}, nullptr, jogador1, jogador2);
        if (kv.find("vidas") != kv.end()) pG->setVidas(std::stoi(kv.at("vidas")));
        if (kv.find("spritePosX") != kv.end() && kv.find("spritePosY") != kv.end()) {
            float sx = std::stof(kv.at("spritePosX"));
            float sy = std::stof(kv.at("spritePosY"));
            pG->setPosition({sx, sy});
        }

        pG->setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico::getInstance());
        listaEntidades.incluir(static_cast<Entidade*>(pG));
    }

    void Fase::restaurarDragao(const std::map<std::string, std::string>& kv)
    {
        using namespace Entidades;

        float x = 0.f, y = 0.f;
        if (kv.find("posX") != kv.end()) x = std::stof(kv.at("posX"));
        if (kv.find("posY") != kv.end()) y = std::stof(kv.at("posY"));

        Personagens::Dragao* pD = new Personagens::Dragao({x, y}, jogador1, jogador2, &listaEntidades);
        if (kv.find("vidas") != kv.end()) pD->setVidas(std::stoi(kv.at("vidas")));
        if (kv.find("spritePosX") != kv.end() && kv.find("spritePosY") != kv.end()) {
            float sx = std::stof(kv.at("spritePosX"));
            float sy = std::stof(kv.at("spritePosY"));
            pD->setPosition({sx, sy});
        }

        pD->setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico::getInstance());
        listaEntidades.incluir(static_cast<Entidade*>(pD));
    }

    void Fase::tratarEvento(const sf::Event& evento)
    {
        // -----------------------------------------------
        // CASO 1: DIGITANDO AS INICIAIS (RANKING)
        // -----------------------------------------------
        if (estadoFim == EstadoFim::PedindoIniciais)
        {
            // Usamos "TextEntered" para capturar caracteres digitados (letras, números, símbolos)
            if (const auto* te = evento.getIf<sf::Event::TextEntered>())
            {
                // O SFML retorna o código Unicode
                std::uint32_t unicode = te->unicode;

                // --- 1. Lógica de Apagar (Backspace - ASCII 8) ---
                if (unicode == 8) 
                {
                    if (!iniciais.empty()) {
                        iniciais.pop_back(); // Remove a última letra
                    }
                }
                // --- 2. Lógica de Confirmar (Enter - ASCII 13) ---
                else if (unicode == 13) 
                {
                    // Debounce: evita múltiplas gravações se o Enter gerar
                    // tanto TextEntered quanto KeyPressed no mesmo frame.
                    if (iniciais.length() == 3 && !enterPressionado) // Só aceita se tiver 3 letras
                    {
                        if (pJogo) {
                            pJogo->adicionarAoRanking(faseNum, iniciais, pontuacaoFinalCache);
                        }
                        faseConcluida = true;
                        enterPressionado = true;
                    }
                }
                // --- 3. Lógica de Digitar (Caracteres Normais) ---
                else 
                {
                    // Aceita apenas se tiver menos de 3 letras
                    if (iniciais.length() < 3)
                    {
                        // Filtra: Apenas letras (A-Z, a-z) e Números (0-9)
                        // (ASCII: 48-57 são números, 65-90 maiúsculas, 97-122 minúsculas)
                        if ((unicode >= 48 && unicode <= 57) || 
                            (unicode >= 65 && unicode <= 90) || 
                            (unicode >= 97 && unicode <= 122))
                        {
                            char letra = static_cast<char>(unicode);
                        
                            // Converte para MAIÚSCULA antes de adicionar
                            iniciais += std::toupper(letra);
                        }
                    }
                }

                // Atualiza o texto visual imediatamente
                if (inputIniciaisText) {
                    inputIniciaisText->setString("Iniciais (3): " + iniciais);
                }
            }
            // Também aceitamos Enter via KeyPressed (algumas plataformas/teclados
            // podem não enviar TextEntered para a tecla Enter em certos contextos)
            else if (const auto* kp = evento.getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Enter)
                {
                    // Usar debouncing para o Enter: só grava se não estiver marcado
                    // como "pressionado" anteriormente.
                    if (iniciais.length() == 3 && !enterPressionado)
                    {
                        if (pJogo) pJogo->adicionarAoRanking(faseNum, iniciais, pontuacaoFinalCache);
                        faseConcluida = true;
                        enterPressionado = true;
                    }
                }
            }
        }

        // -----------------------------------------------
        // CASO 2: MENU DE FIM DE FASE (SALVAR/VOLTAR)
        // -----------------------------------------------
        else if (estadoFim == EstadoFim::MostrandoOpcoes)
        {
            if (const auto* kp = evento.getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Up || kp->code == sf::Keyboard::Key::W)
                {
                    posBotaoFim = 0; // Vai para "Salvar"
                }
                else if (kp->code == sf::Keyboard::Key::Down || kp->code == sf::Keyboard::Key::S)
                {
                    posBotaoFim = 1; // Vai para "Menu"
                }
                else if (kp->code == sf::Keyboard::Key::Enter)
                {
                    if (posBotaoFim == 0) // Salvar
                    {
                        // Ao entrar no modo de digitar iniciais, garantimos que o
                        // debounce do Enter esteja liberado para permitir a primeira
                        // confirmação.
                        estadoFim = EstadoFim::PedindoIniciais;
                        iniciais = "";
                        enterPressionado = false;
                        // Atualiza o texto visual para limpar
                        if (inputIniciaisText) inputIniciaisText->setString("Iniciais (3): ");
                    }
                    else // Menu
                    {
                        faseConcluida = true;
                    }
                }
            }
        }

        // -----------------------------------------------
        // CASO 3: JOGO RODANDO (PULO DO JOGADOR)
        // -----------------------------------------------
        else if (jogador1)
        {
            if (const auto* kp = evento.getIf<sf::Event::KeyPressed>())
            {
                if (kp->code == sf::Keyboard::Key::Space)
                {
                    jogador1->pular(); 
                }
            }
        }
    }

}