#pragma once

#include "GerenciadorGrafico.hpp"
#include "Jogador.hpp"
#include <SFML/Graphics.hpp>

class Jogo
{
private:
    GerenciadorGrafico gerenciadorGrafico;
    Jogador jogador1;
    sf::Clock relogio; // (Se você já adicionou isso, ótimo)

    // O TESTE:
    sf::RectangleShape testeQuadrado; // <-- ADICIONE ISTO

    void processarEventos();
    void atualizar(); // (ou void atualizar();)
    void renderizar();

public:
    Jogo();
    ~Jogo();

    void executar();
};