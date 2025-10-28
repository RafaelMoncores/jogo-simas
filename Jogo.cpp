#include "Jogo.hpp"


Jogo::Jogo() :
    gerenciadorGrafico(),
    jogador1()
{
}

Jogo::~Jogo()
{
}

void Jogo::executar()
{
    while (gerenciadorGrafico.isWindowOpen())
    {
        processarEventos();
        atualizar();
        renderizar();
    }
}

void Jogo::processarEventos()
{
    while (auto event = gerenciadorGrafico.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            gerenciadorGrafico.fecharWindow();
        }
    }
}

void Jogo::atualizar()
{
    jogador1.atualizar();
    
    // Se tivéssemos 100 inimigos, faríamos um loop aqui:
    // for (auto& inimigo : listaDeInimigos)
    // {
    //     inimigo.atualizar();
    // }
}

void Jogo::renderizar()
{
    gerenciadorGrafico.limpar();
    jogador1.desenhar(gerenciadorGrafico);

    // Se tivéssemos inimigos:
    // for (auto& inimigo : listaDeInimigos)
    // {
    //     inimigo.desenhar(gerenciadorGrafico);
    // }

    gerenciadorGrafico.exibir();
}