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

    // Loop de polling de eventos centralizado
    void GerenciadorEventos::processarEventos() {
        //Pede ao Gerenciador Gráfico para verificar se tem eventos na fila da janela
        while (auto optEvent = pGG->pollEvent()) {
            //Extrai o evento de dentro do "optional"
            const auto& evento = *optEvent;

            // Tratamento prioritário: Fechar janela (funciona em qualquer tela)
            if (evento.is<sf::Event::Closed>()) {
                pGG->fecharWindow();
            }

            // Delegação: Repassa o evento específico para a tela atual lidar
            if (pOuvinteAtual) {
                pOuvinteAtual->tratarEvento(evento);
            }
        }
    }
}