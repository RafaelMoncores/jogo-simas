#pragma once

#include "Gerenciadores/GerenciadorGrafico.hpp"
#include "Entidades/Personagens/Jogador.hpp"
#include "Estados/Menu.hpp"
#include <SFML/System/Clock.hpp>

class Jogo
{
    private:
        enum class EstadoJogo { NoMenu, Jogando, Pausado };
        
        EstadoJogo estadoAtual;
        Gerenciadores::GerenciadorGrafico* pGG;
        Entidades::Personagens::Jogador jogador1;
        Estados::Menu menu;
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