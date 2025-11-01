#include "Jogador.hpp"
#include <iostream>
#include <stdlib.h> 

Jogador::Jogador() :
    Entidade() 
{
    if (!textura.loadFromFile("player.png"))
    {
        std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
        exit(1);
    }

    sprite.emplace(textura);
    sprite->setPosition({400.0f, 300.0f}); 
}

Jogador::~Jogador()
{
}

void Jogador::aplicarGravidade()
{
    if (sprite) 
    {
        velocidade.y += G_ACCEL.y;
        sprite->move(velocidade);
    }
}

void Jogador::executar()
{   
    // TODO: Input de movimento
}

void Jogador::desenhar()
{
    if (pGG && sprite)
    {
        pGG->desenhar(*sprite);
    }
}