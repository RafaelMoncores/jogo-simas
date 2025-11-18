#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Jogo.hpp"
#include "../Entidades/BolaDeFogo.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdint>

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
        enterPressionado(true)
    {
        inicializarUI();
    }

    Fase::~Fase()
    {   
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            delete pE;
        }
        
        listaObstaculos.limpar();
        listaInimigos.limpar();
        listaEntidades.limpar();
        
        jogador1 = nullptr;
        jogador2 = nullptr;
    }

    void Fase::inicializarUI()
    {
        if (!uiFont.openFromFile("font/PressStart2P-Regular.ttf"))
        {
            std::cerr << "ERRO: Nao foi possivel carregar a fonte 'PressStart2P-Regular.ttf'" << std::endl;
        }

        vidasTextP1.emplace(uiFont);
        vidasTextP1->setCharacterSize(30);
        vidasTextP1->setFillColor(sf::Color::White);
        vidasTextP1->setPosition({60.f, 10.f}); 
        vidasTextP1->setString("P1 Vidas: ");

        pontosTextP1.emplace(uiFont);
        pontosTextP1->setCharacterSize(30);
        pontosTextP1->setFillColor(sf::Color::White);
        sf::FloatRect boundsPontos = pontosTextP1->getLocalBounds();
        pontosTextP1->setOrigin({boundsPontos.size.x / 2.f, 0.f});
        pontosTextP1->setPosition({760, 10.f});
        pontosTextP1->setString("P1 Pontos: ");

        vidasTextP2.emplace(uiFont);
        vidasTextP2->setCharacterSize(30);
        vidasTextP2->setFillColor(sf::Color::White);
        vidasTextP2->setPosition({60.f, 50.f});
        vidasTextP2->setString("P2 Vidas: ");

        pontosTextP2.emplace(uiFont);
        pontosTextP2->setCharacterSize(30);
        pontosTextP2->setFillColor(sf::Color::White);
        sf::FloatRect boundsPontosP2 = pontosTextP2->getLocalBounds();
        pontosTextP2->setOrigin({boundsPontosP2.size.x / 2.f, 0.f});
        pontosTextP2->setPosition({760, 50.f});
        pontosTextP2->setString("P2 Pontos: ");

        tentativasText.emplace(uiFont); 
        tentativasText->setCharacterSize(30);
        tentativasText->setFillColor(sf::Color::White);
        sf::FloatRect bounds = tentativasText->getLocalBounds();
        tentativasText->setOrigin({bounds.size.x, 0.f});
        tentativasText->setPosition({1850.f, 10.f}); 
        tentativasText->setString("tentativas: ");

        pontuacaoFinalText.emplace(uiFont); 
        pontuacaoFinalText->setCharacterSize(60);
        pontuacaoFinalText->setFillColor(sf::Color::White);
        pontuacaoFinalText->setOrigin({pontuacaoFinalText->getLocalBounds().size.x / 2.f,pontuacaoFinalText->getLocalBounds().size.y / 2.f});
        pontuacaoFinalText->setPosition({960.f, 540.f}); 
        pontuacaoFinalText->setString("Pontuacao: ");

        // Botão Salvar
        botaoSalvarText.emplace(uiFont);
        botaoSalvarText->setCharacterSize(40);
        botaoSalvarText->setString("Salvar no Ranking");
        sf::FloatRect boundsSalvar = botaoSalvarText->getLocalBounds();
        botaoSalvarText->setOrigin({boundsSalvar.size.x / 2.f, boundsSalvar.size.y / 2.f});
        botaoSalvarText->setPosition({1920.f / 2.f, (1080.f / 2.f) + 100.f}); // Abaixo da pontuação

        // Botão Menu
        botaoMenuText.emplace(uiFont);
        botaoMenuText->setCharacterSize(40);
        botaoMenuText->setString("Menu");
        sf::FloatRect boundsMenu = botaoMenuText->getLocalBounds();
        botaoMenuText->setOrigin({boundsMenu.size.x / 2.f, boundsMenu.size.y / 2.f});
        botaoMenuText->setPosition({1920.f / 2.f, (1080.f / 2.f) + 160.f}); // Abaixo do Salvar

        // Texto de Input
        inputIniciaisText.emplace(uiFont);
        inputIniciaisText->setCharacterSize(40);
        inputIniciaisText->setString("Iniciais (3): ___");
        sf::FloatRect boundsInput = inputIniciaisText->getLocalBounds();
        inputIniciaisText->setOrigin({boundsInput.size.x / 2.f, boundsInput.size.y / 2.f});
        inputIniciaisText->setPosition({1920.f / 2.f, (1080.f / 2.f) + 100.f});
    }
    
    void Fase::inicializar()
    {
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            delete pE;
        }
        
        listaObstaculos.limpar();
        listaInimigos.limpar();
        listaEntidades.limpar();
        
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

    void Fase::processarEvento(const sf::Event& evento)
    {
        bool p1Completo = (jogador1 && jogador1->getCompletouFase());
        bool p2Completo = (modoDoisJogadores && jogador2 && jogador2->getCompletouFase());

        if ((p1Completo || p2Completo) && estadoFim == EstadoFim::PedindoIniciais)
        {
            processarInputIniciais(evento);
        }
    }

    void Fase::processarInputIniciais(const sf::Event& evento)
    {
        if (evento.is<sf::Event::TextEntered>())
        {
            const auto* te = evento.getIf<sf::Event::TextEntered>();
            if (!te) return;

            std::uint32_t valorUnicode = te->unicode;

            if (valorUnicode == 8)
            {
                if (!iniciais.empty())
                {
                    iniciais.pop_back();
                }
            }
            else if (valorUnicode >= 32 && valorUnicode <= 126 && iniciais.length() < 3)
            {
                char c = static_cast<char>(valorUnicode);
                iniciais += std::toupper(c);
            }
            
            if (inputIniciaisText)
            {
                std::string placeholder = "___";
                std::string textoAtual = iniciais;
                if (textoAtual.length() < 3)
                {
                    textoAtual += placeholder.substr(textoAtual.length());
                }
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

        if (p1Completo || p2Completo)
        {
            if (pontuacaoFinalCache == -1) 
            {
                if (p1Completo)
                    pontuacaoFinalCache = jogador1->getPontos();
                else
                    pontuacaoFinalCache = jogador2->getPontos();
            
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
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        if (!navPressionado) {
                            posBotaoFim = 0;
                            navPressionado = true;
                        }
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    {
                        if (!navPressionado) {
                            posBotaoFim = 1;
                            navPressionado = true;
                        }
                    }
                    
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        if (!enterPressionado)
                        {
                            if (posBotaoFim == 0) 
                            {
                                estadoFim = EstadoFim::PedindoIniciais;
                                iniciais = ""; 
                            }
                            else 
                            {
                                faseConcluida = true; 
                            }
                            enterPressionado = true;
                        }
                    }
                    else
                    {
                        enterPressionado = false;
                        navPressionado = false; 
                    }

                }
                break;

                case EstadoFim::PedindoIniciais:
                {
                    
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !enterPressionado)
                    {
                        if (iniciais.length() == 3)
                        {
                            if (pJogo)
                                pJogo->adicionarAoRanking(faseNum, iniciais, pontuacaoFinalCache);
                            
                            faseConcluida = true; 
                            enterPressionado = true;
                        }
                    }
                    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        enterPressionado = false;
                    }

                }
                break;
            }
            
            return; 
        }

        // --- 2. LÓGICA DE JOGO NORMAL (COLISÕES E UPDATES) ---
        
        gerenciadorColisoes.verificarColisoes(jogador1, &listaObstaculos, &listaInimigos, &listaEntidades);
        
        if (modoDoisJogadores && jogador2)
        {
            gerenciadorColisoes.verificarColisoes(jogador2, &listaObstaculos, &listaInimigos, &listaEntidades);
        }

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->executar(delta); 
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
            tentativasText->setPosition({1850.f, 10.f}); 
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
                    if (pInim->getVidas() <= 0)
                    {
                        deletar = true;
                        listaInimigos.remover(pInim);
                    }
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
                    // Lógica de Destaque para "Salvar"
                    if (botaoSalvarText) 
                    {
                        if (posBotaoFim == 0) { // Se "Salvar" está selecionado
                            botaoSalvarText->setOutlineThickness(4.f);
                            botaoSalvarText->setFillColor(sf::Color::White); // Garante a cor
                        } else {
                            botaoSalvarText->setOutlineThickness(0.f);
                            botaoSalvarText->setFillColor(sf::Color::White);
                        }
                        pGG->desenhar(*botaoSalvarText);
                    }
                    
                    // Lógica de Destaque para "Menu"
                    if (botaoMenuText) 
                    {
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
        // Método de persistência (ainda não implementado)
    }
}