#pragma once

#include <SFML/Window/Event.hpp>
#include "GerenciadorGrafico.hpp"

// Interface que Menu e Fase terão que assinar
class OuvinteEventos {
public:
    virtual void tratarEvento(const sf::Event& evento) = 0;
    virtual ~OuvinteEventos() {}
};

namespace Gerenciadores {

    class GerenciadorEventos {
    private:
        static GerenciadorEventos* pInstancia;
        GerenciadorGrafico* pGG;
        OuvinteEventos* pOuvinteAtual; // Quem está recebendo os inputs agora?

        GerenciadorEventos();

    public:
        ~GerenciadorEventos();
        static GerenciadorEventos* getInstance();

        // Define quem vai receber os cliques e teclas (Menu ou Fase)
        void setOuvinte(OuvinteEventos* ouvinte);

        void processarEventos();
    };

}