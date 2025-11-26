#pragma once

#include <SFML/Window/Event.hpp>
#include "GerenciadorGrafico.hpp"

// Interface que define quem pode responder a inputs
class OuvinteEventos {
public:
    virtual void tratarEvento(const sf::Event& evento) = 0; // Processa eventos da fila da SFML (ex: KeyPressed, KeyReleased, Closed).
    virtual ~OuvinteEventos() {}
};

namespace Gerenciadores {

    class GerenciadorEventos {
    private:
        static GerenciadorEventos* pInstancia;
        GerenciadorGrafico* pGG;
        OuvinteEventos* pOuvinteAtual; // Ponteiro para a tela ativa (Menu ou Fase)

        GerenciadorEventos();

    public:
        ~GerenciadorEventos();
        static GerenciadorEventos* getInstance();

        // Troca o contexto de input (ex: ao sair do menu e entrar no jogo)
        void setOuvinte(OuvinteEventos* ouvinte);

        void processarEventos();
    };

}