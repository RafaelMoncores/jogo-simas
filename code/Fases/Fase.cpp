#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

namespace Fases
{
    Fase::Fase() :
        Ente(),
        jogador1(nullptr),
        faseConcluida(false),
        posJogadorInicial(0.f, 0.f),
        pontuacaoFinalCache(0),
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
    }

    void Fase::inicializarUI()
    {
        if (!uiFont.openFromFile("font/PressStart2P-Regular.ttf"))
        {
            std::cerr << "ERRO: Nao foi possivel carregar a fonte 'PressStart2P-Regular.ttf'" << std::endl;
        }

        vidasText.emplace(uiFont); 
        vidasText->setCharacterSize(30);
        vidasText->setFillColor(sf::Color::White);
        vidasText->setPosition({60.f, 10.f}); 
        vidasText->setString("Vidas: ");

        pontosText.emplace(uiFont); 
        pontosText->setCharacterSize(30);
        pontosText->setFillColor(sf::Color::White);
        sf::FloatRect boundsPontos = pontosText->getLocalBounds();
        pontosText->setOrigin({boundsPontos.size.x / 2.f, 0.f}); // Sintaxe SFML 3.x
        pontosText->setPosition({760, 10.f});
        pontosText->setString("Pontos: ");

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

        Gerenciadores::GerenciadorGrafico::getInstance()->setViewBounds(0.f, 0.f, 1920.f, 1080.f);

        jogador1 = new Entidades::Personagens::Jogador(posJogadorInicial);
        jogador1->setPosition(posJogadorInicial);
        
        listaEntidades.incluir(jogador1); 

        criarObstaculos();
        criarInimigos();
    }
    
    void Fase::executar(float delta)
    {
        // --- 1. LÓGICA DE FASE CONGELADA (NO TOPO) ---
        // Se o jogador pisou na plataforma final...
        if (jogador1 && jogador1->getCompletouFase())
        {
            // 1a. Salva a pontuação (só na primeira vez)
            if (pontuacaoFinalCache == 0 && jogador1->getPontos() > 0) 
            {
                pontuacaoFinalCache = jogador1->getPontos();
                if (pontuacaoFinalText)
                {
                    pontuacaoFinalText->setString("PONTUACAO: " + std::to_string(pontuacaoFinalCache));
                    
                    // Re-centraliza o texto
                    sf::FloatRect bounds = pontuacaoFinalText->getLocalBounds();
                    pontuacaoFinalText->setOrigin({pontuacaoFinalText->getLocalBounds().size.x / 2.f,pontuacaoFinalText->getLocalBounds().size.y / 2.f});
                    pontuacaoFinalText->setPosition({1920.0f / 2.f, 1080.0f / 2.f});
                }
            }

            // 1b. Processa o input para voltar ao menu
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
            {
                if (!enterPressionado)
                {
                    faseConcluida = true; // <-- SINALIZA PARA O JOGO VOLTAR AO MENU
                }
            }
            else
            {
                enterPressionado = false; // Usuário soltou o ENTER (debounce)
            }
            
            // 1c. (congela o jogo)
            return; 
        }

        // --- 2. LÓGICA DE JOGO NORMAL ---
        
        gerenciadorColisoes.verificarColisoes(jogador1, &listaObstaculos, &listaInimigos);

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->executar(delta); 
        }

        // --- 3. ATUALIZAÇÃO DA UI (Vidas e Pontos Normais) ---
        if (jogador1)
        {
            if (vidasText)
            {
                vidasText->setString("Vidas: " + std::to_string(jogador1->getVidas()));
            }

            if (pontosText)
            {
                pontosText->setString("Pontos: " + std::to_string(jogador1->getPontos()));
            }

            if (tentativasText)
            {
                tentativasText->setString("Tentativas: " + std::to_string(numTentativas));
                sf::FloatRect bounds = tentativasText->getLocalBounds();
                // Alinha a origem ao canto superior DIREITO do texto
                tentativasText->setOrigin({bounds.size.x, 0.f});
                tentativasText->setPosition({1850.f, 10.f}); 
            }

            if (jogador1->getVidas() <= 0)
            {
                std::cout << "Sem vidas! Reiniciando a fase..." << std::endl;
                
                numTentativas++; // Incrementa as tentativas

                int pontosIniciais = 1000 - ((numTentativas - 1) * 100);
                
                // Garante que a pontuação não é negativa
                if (pontosIniciais < 0) pontosIniciais = 0; 
                
                // Reinicia a fase (isso vai recriar o jogador)
                inicializar(); 
                
                // Define os pontos do *novo* jogador
                if (jogador1) 
                {
                    jogador1->setPontos( (float)pontosIniciais );
                }
            }
        }
        else if (vidasText)
        {
            vidasText->setString("Vidas: 0");
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
            // JOGO CONGELADO: Desenha SÓ a pontuação final
            if (pontuacaoFinalText) pGG->desenhar(*pontuacaoFinalText);
        }
        else
        {
            // JOGO NORMAL: Desenha a UI normal
            if (vidasText) pGG->desenhar(*vidasText);
            if (pontosText) pGG->desenhar(*pontosText); 
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