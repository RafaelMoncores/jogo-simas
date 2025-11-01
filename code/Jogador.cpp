#include "Jogador.hpp"
#include <iostream>
#include <stdlib.h>
#include <SFML/Window/Keyboard.hpp>

Jogador::Jogador() :
    Entidade(),
    VELOCIDADE_LATERAL(0.5f),
    FORCA_PULO(1.2f),
    podePular(false)
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

void Jogador::processarInputs()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        velocidade.x -= VELOCIDADE_LATERAL;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        velocidade.x += VELOCIDADE_LATERAL;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && podePular)
    {
        velocidade.y = -FORCA_PULO; 
        podePular = false;
    }
}

void Jogador::aplicarGravidade()
{
    if (sprite) 
    {
        velocidade.y += G_ACCEL.y;
        sprite->move(velocidade);

        // --- HACK: Simulação de Chão (TEMPORÁRIO) ---
        // Isso é necessário para testar o pulo.
        // Vamos fingir que o chão está na posição Y = 800
        
        float chaoFalso = 800.0f;
        sf::Vector2f pos = sprite->getPosition();

        if (pos.y > chaoFalso)
        {
            sprite->setPosition({pos.x, chaoFalso});
            velocidade.y = 0.0f;
            podePular = true; 
        }
    }
}

void Jogador::executar()
{   
    velocidade.x = 0.0f;
    processarInputs();
    aplicarGravidade();
}

void Jogador::desenhar()
{
    if (pGG && sprite)
    {
        pGG->desenhar(*sprite);
    }
}