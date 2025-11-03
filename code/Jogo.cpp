#include "Jogo.hpp"
#include <iostream>
#include "Entidades/Ente.hpp"

using Gerenciadores::GerenciadorGrafico;
using Entidades::Ente;
//using Entidades::Personagens::Jogador;
using Estados::Menu;
using Fases::Fase;

Jogo::Jogo() :
    pGG(nullptr),
    //jogador1(),
    menu(),
    pFaseAtual(nullptr),
    estadoAtual(EstadoJogo::NoMenu)
    //texFase(),      
    //bgFase()        
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
    
    Ente::setGerenciadorGrafico(pGG);

    
    /*if (!texFase.loadFromFile("fase_background.png"))
    {
        std::cerr << "Erro: nao foi possivel carregar imagem fase_background.png\n";
    }
    else
    {
        bgFase.emplace(texFase);
        
        
        float scaleX = 1920.f / texFase.getSize().x;
        float scaleY = 1080.f / texFase.getSize().y;

        
        bgFase->setScale(sf::Vector2f(scaleX, scaleY));
        
        
    }*/
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
                int acaoMenu = menu.executar();
                if (acaoMenu == 1)
                {
                    estadoAtual = EstadoJogo::Jogando;
                    if (!pFaseAtual)
                    {
                        pFaseAtual = new Fase();
                    }
                    // Inicializa (ou reinicializa) a fase
                    pFaseAtual->inicializar(); 
                    estadoAtual = EstadoJogo::Jogando;
                    // --- FIM ---
                }
                else if (acaoMenu == 0)
                {
                    pGG->fecharWindow();
                }
            }
            break;
        
            case EstadoJogo::Jogando:
            {
                processarEventosJogando();
                
                //atualizar(delta);

                if (pFaseAtual)
                {
                    pFaseAtual->executar(delta); // A fase agora cuida da atualização
                }
                
                renderizar(); 
            }
            break;

            case EstadoJogo::Pausado:
                // TODO
            break;
        }

        pGG->exibir(); 
    }
}

void Jogo::processarEventosJogando()
{
    while (auto event = pGG->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            pGG->fecharWindow();
        }
    }
}

/*void Jogo::atualizar(float delta)
{
    jogador1.executar(delta);
}*/

void Jogo::renderizar()
{
    /*
    if (bgFase)
    {
        pGG->desenhar(*bgFase);
    }
    
    jogador1.desenhar();
    */

    if (pFaseAtual)
    {
        pFaseAtual->desenhar(); // A fase agora cuida de desenhar
    }
}