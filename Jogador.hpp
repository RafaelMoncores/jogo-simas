#pragma once

#include <SFML/Graphics.hpp>
#include "GerenciadorGrafico.hpp"

class Jogador
{
private:
    sf::Texture textura;
    sf::Sprite sprite;

public:
    Jogador();
    ~Jogador();

    void atualizar();
    
    void desenhar(GerenciadorGrafico& gerenciador); 
};