#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Jogo.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

namespace Fases
{
    Fase::Fase(Jogo* pJ) :
        Ente(),
        pJogo(pJ),
        jogador1(nullptr),
        faseConcluida(false),
        estadoFim(EstadoFim::MostrandoOpcoes),
        posJogadorInicial(0.f, 0.f),
        pontuacaoFinalCache(-1),
        posBotaoFim(0),
        iniciais(""),
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
            if (pontuacaoFinalCache == -1) 
            {
                pontuacaoFinalCache = jogador1->getPontos(); 
            
                if (pontuacaoFinalText)
                {
                    pontuacaoFinalText->setString("PONTUACAO: " + std::to_string(pontuacaoFinalCache));
                    
                    // Re-centraliza (com a sua sintaxe SFML 3.x)
                    sf::FloatRect bounds = pontuacaoFinalText->getLocalBounds();
                    pontuacaoFinalText->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
                    // (Não precisa de setPosition, pois já foi feito no inicializarUI)
                }
            }

            // 1b. Sub-máquina de estados do fim da fase
            switch (estadoFim)
            {
                case EstadoFim::MostrandoOpcoes:
                {
                    // --- Processar Inputs de Navegação (Cima/Baixo) ---
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        if (!enterPressionado) {
                            posBotaoFim = 0; // Vai para "Salvar"
                            enterPressionado = true;
                        }
                    }
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    {
                        if (!enterPressionado) {
                            posBotaoFim = 1; // Vai para "Menu"
                            enterPressionado = true;
                        }
                    }
                    
                    // --- Processar Seleção (ENTER) ---
                    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        if (!enterPressionado)
                        {
                            if (posBotaoFim == 0) // Selecionou "Salvar"
                            {
                                estadoFim = EstadoFim::PedindoIniciais;
                                iniciais = ""; // Reseta as iniciais
                            }
                            else // Selecionou "Menu"
                            {
                                faseConcluida = true; // Sinaliza para o Jogo voltar ao Menu
                            }
                            enterPressionado = true;
                        }
                    }
                    else
                    {
                        enterPressionado = false; // Debounce
                    }

                    // --- Atualização Visual dos Botões (feedback) ---
                    if (botaoSalvarText)
                        botaoSalvarText->setFillColor(posBotaoFim == 0 ? sf::Color::White : sf::Color::White);
                    if (botaoMenuText)
                        botaoMenuText->setFillColor(posBotaoFim == 1 ? sf::Color::White : sf::Color::White);
                }
                break;

                case EstadoFim::PedindoIniciais:
                {
                    // --- Processar Input de Texto ---
                    // Esta é uma lógica de input de texto simplificada.
                    // Para uma versão completa, precisaríamos de um loop de eventos.
                    // Por agora, vamos apenas simular a captura e salvar.

                    // (Simulação simplificada: captura 3 letras e salva)
                    if (iniciais.length() < 3)
                    {
                        // LÓGICA SIMPLIFICADA: apenas aceita "AAA" e salva
                        // (Input de texto real em C++ é complexo,
                        // recomendo focar nisto *depois* do resto funcionar)
                        
                        // Vamos apenas usar "AAA" como placeholder
                        iniciais = "AAA"; 
                        if (inputIniciaisText)
                            inputIniciaisText->setString("Iniciais (3): " + iniciais);
                    }
                    
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter) && !enterPressionado)
                    {
                        if (iniciais.length() == 3)
                        {
                            // --- SALVA NO RANKING ---
                            if (pJogo)
                                pJogo->adicionarAoRanking(iniciais, pontuacaoFinalCache);
                            
                            faseConcluida = true; // Volta ao menu
                            enterPressionado = true;
                        }
                    }
                    else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        enterPressionado = false;
                    }

                    // Atualiza o texto de input (na implementação real,
                    // você atualizaria o string 'iniciais' com base nas teclas)
                    if (inputIniciaisText)
                        inputIniciaisText->setString("Iniciais (3): " + iniciais + "_");

                }
                break;
            }
            
            // 3. NÃO EXECUTA MAIS NADA (congela o jogo)
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
            // Desenha a pontuação final (sempre)
            if (pontuacaoFinalText) pGG->desenhar(*pontuacaoFinalText);

            switch (estadoFim)
            {
                case EstadoFim::MostrandoOpcoes:
                    if (botaoSalvarText) pGG->desenhar(*botaoSalvarText);
                    if (botaoMenuText) pGG->desenhar(*botaoMenuText);
                    break;
                
                case EstadoFim::PedindoIniciais:
                    if (inputIniciaisText) pGG->desenhar(*inputIniciaisText);
                    break;
            }
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