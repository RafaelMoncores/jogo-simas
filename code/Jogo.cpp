#include "Jogo.hpp"
#include <iostream>
#include "Entidades/Ente.hpp"

using Gerenciadores::GerenciadorGrafico;
using Entidades::Ente;
using Entidades::Personagens::Jogador;
using Estados::Menu;

Jogo::Jogo() :
    pGG(nullptr),
    jogador1(),
    menu(),
    estadoAtual(EstadoJogo::NoMenu) 
{
    inicializar();
}

Jogo::~Jogo()
{
    delete GerenciadorGrafico::getInstance();
}

void Jogo::inicializar()
{
    pGG = GerenciadorGrafico::getInstance();
    
    Ente::setGerenciadorGrafico(pGG);
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
                
                atualizar(delta);
                
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

void Jogo::atualizar(float delta)
{
    jogador1.executar(delta);
}

void Jogo::renderizar()
{
    jogador1.desenhar();
}