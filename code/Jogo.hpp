#pragma once

#include "Gerenciadores/GerenciadorGrafico.hpp"
#include "Entidades/Personagens/Jogador.hpp"
#include "Estados/Menu.hpp"
#include "Fases/Fase.hpp"
#include <SFML/System/Clock.hpp>

class Jogo
{
    private:
        enum class EstadoJogo { NoMenu, Jogando, Pausado };
        
        EstadoJogo estadoAtual;
        Gerenciadores::GerenciadorGrafico* pGG;
        Estados::Menu menu;
        Fases::Fase* pFaseAtual;
        sf::Clock relogio;

        void processarEventosJogando(); 
        void renderizar();
        void inicializar();

    public:
        Jogo();
        ~Jogo();

        void executar();
};