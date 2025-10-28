#pragma once

#include "GerenciadorGrafico.hpp"
#include "Jogador.hpp"

class Jogo
{
private:
    GerenciadorGrafico gerenciadorGrafico;
    Jogador jogador1;

    void processarEventos();
    void atualizar();
    void renderizar();

public:
    Jogo();
    ~Jogo();

    void executar();
};