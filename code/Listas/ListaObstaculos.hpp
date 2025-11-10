# pragma once

#include "Lista.hpp"

// Forward declaration para o tipo
namespace Entidades { namespace Obstaculos { class Obstaculo; } }

namespace Listas 
{
    class ListaObstaculos {
    private:
        // LOs: Lista de Obstaculos (conforme atributos do UML)
        Lista<Entidades::Obstaculos::Obstaculo> LOs; 

    public:
        ListaObstaculos();
        ~ListaObstaculos();

        void incluir(Entidades::Obstaculos::Obstaculo* pO);
        void limpar();

        // Métodos de iteração
        void irParaPrimeiro();
        void irParaProximo();
        Entidades::Obstaculos::Obstaculo* getAtual() const;
    };
}