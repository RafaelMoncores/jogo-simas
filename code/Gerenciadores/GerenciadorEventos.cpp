#include "GerenciadorEventos.hpp"
#include <iostream>

namespace Gerenciadores {

    GerenciadorEventos* GerenciadorEventos::pInstancia = nullptr;

    GerenciadorEventos::GerenciadorEventos() :
        pGG(GerenciadorGrafico::getInstance()),
        pOuvinteAtual(nullptr)
    {
    }

    GerenciadorEventos::~GerenciadorEventos() {
        pOuvinteAtual = nullptr;
    }

    GerenciadorEventos* GerenciadorEventos::getInstance() {
        if (pInstancia == nullptr) {
            pInstancia = new GerenciadorEventos();
        }
        return pInstancia;
    }

    void GerenciadorEventos::setOuvinte(OuvinteEventos* ouvinte) {
        pOuvinteAtual = ouvinte;
    }

    void GerenciadorEventos::processarEventos() {
        // Usa o pollEvent do GerenciadorGrafico
        while (auto optEvent = pGG->pollEvent()) {
            const auto& evento = *optEvent;

            // Tratamento Global (Fechar Janela)
            if (evento.is<sf::Event::Closed>()) {
                pGG->fecharWindow();
            }

            // Repassa para o ouvinte atual (Menu ou Fase)
            if (pOuvinteAtual) {
                pOuvinteAtual->tratarEvento(evento);
            }
        }
    }
}