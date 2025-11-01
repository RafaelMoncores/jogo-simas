#pragma once

#include "GerenciadorGrafico.hpp"
#include "Jogador.hpp"
#include "Menu.hpp" 

class Jogo
{
private:
    enum class EstadoJogo { NoMenu, Jogando, Pausado };
    
    EstadoJogo estadoAtual;
    GerenciadorGrafico* pGG;
    Jogador jogador1;
    Menu menu;

    void processarEventosJogando(); 
    void atualizar();
    void renderizar();

    void inicializar();

public:
    Jogo();
    ~Jogo();

    void executar();
};