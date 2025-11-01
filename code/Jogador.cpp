#include "Jogador.hpp"
#include <iostream>
#include <stdlib.h>

Jogador::Jogador() : textura("player.png"), sprite(textura)
{
    if (!textura.loadFromFile("player.png"))
    {
        std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
        std::cerr << "Verifique se o arquivo esta na pasta raiz do projeto." << std::endl;
        exit(1); 
    }

    sprite.setTexture(textura);

    // formaDoJogador.setSize(sf::Vector2f(60.f, 60.f)); // Tamanho de 60x60 pixels
    // formaDoJogador.setFillColor(sf::Color::Black);  // Cor preta
    // formaDoJogador.setPosition(sf::Vector2f(400.f, 300.f));

    sf::Vector2u tamanhoTextura = textura.getSize();
    std::cout << ">>> DEBUG: Textura carregada, tamanho: (" << tamanhoTextura.x << ", " << tamanhoTextura.y << ")" << std::endl;
    if (tamanhoTextura.x == 0 || tamanhoTextura.y == 0)
    {
        std::cerr << "ERRO FATAL: Textura carregou, mas o tamanho e (0, 0)." << std::endl;
        exit(2); 
    }

    sprite.setTexture(textura);
    
    sprite.setPosition(sf::Vector2f(400.f, 300.f));
}

Jogador::~Jogador()
{
}

void Jogador::atualizar()
{
    // Vazio por enquanto. Aqui entrará a lógica de movimento.
}

void Jogador::desenhar(GerenciadorGrafico& gerenciador)
{
    sf::Vector2f pos = formaDoJogador.getPosition(); // Pegue a posição da forma, não do sprite
    std::cout << "  -> Desenhando jogador na posicao: (" << pos.x << ", " << pos.y << ")" << std::endl;
    
    gerenciador.desenhar(sprite); // Desenhe a forma, não o sprite
}