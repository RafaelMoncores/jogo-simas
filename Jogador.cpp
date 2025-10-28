#include "Jogador.hpp"
#include <iostream>

Jogador::Jogador(): sprite(textura)
{
    if (!textura.loadFromFile("player.png"))
    {
        std::cerr << "ERRO: Nao foi possivel carregar 'player.png' para o Jogador" << std::endl;
    }
    
    sprite.setPosition(sf::Vector2f(400.f, 300.f));
}

Jogador::~Jogador()
{
}

void Jogador::atualizar()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
    {
        sprite.move({0.f, -1.f}); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        sprite.move({0.f, 1.f}); 
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        sprite.move({-1.f, 0.f});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        sprite.move({1.f, 0.f}); 
    }
}

void Jogador::desenhar(GerenciadorGrafico& gerenciador)
{
    gerenciador.desenhar(sprite);
}