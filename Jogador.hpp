#pragma once

#include <SFML/Graphics.hpp>
#include "GerenciadorGrafico.hpp"

class Jogador
{
private:
    sf::Texture textura;
    sf::Sprite sprite;

    sf::RectangleShape formaDoJogador;

public:
    Jogador();
    ~Jogador();

    void atualizar();
    void desenhar(GerenciadorGrafico& gerenciador);
};