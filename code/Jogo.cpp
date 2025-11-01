#include "Jogo.hpp"
#include <iostream>

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

        switch (estadoAtual)
        {
            case EstadoJogo::NoMenu:
            {
                int acaoMenu = menu.executar();
                if (acaoMenu == 1) // "Jogar"
                {
                    estadoAtual = EstadoJogo::Jogando;
                }
                else if (acaoMenu == 0) // "Sair"
                {
                    pGG->fecharWindow();
                }
                // Se for -1, continua no menu
            }
            break;
        
            case EstadoJogo::Jogando:
            {
                processarEventosJogando();
                atualizar();  
                renderizar(); 
            }
            break;

            case EstadoJogo::Pausado:
                // TODO
            break;
        }

        // 3. Exibe o frame
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

void Jogo::atualizar()
{
    jogador1.executar();
}

void Jogo::renderizar()
{
    jogador1.desenhar();
}