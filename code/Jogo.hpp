#pragma once

#include "GerenciadorGrafico.hpp"
#include "Jogador.hpp"
#include "Menu.hpp" 
#include <SFML/System/Clock.hpp>

class Jogo
{
    private:
        enum class EstadoJogo { NoMenu, Jogando, Pausado };
        
        EstadoJogo estadoAtual;
        GerenciadorGrafico* pGG;
        Jogador jogador1;
        Menu menu;
        sf::Clock relogio;

        void processarEventosJogando(); 
        void atualizar(float delta);
        void renderizar();

        void inicializar();

    public:
        Jogo();
        ~Jogo();

        void executar();
};