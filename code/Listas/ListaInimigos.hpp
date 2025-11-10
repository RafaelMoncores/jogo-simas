#pragma once

#include "Lista.hpp"

// Forward declaration para o tipo
namespace Entidades { namespace Personagens { class Inimigo; } }

namespace Listas 
{
    class ListaInimigos {
    private:
        // LIs: Lista de Inimigos (conforme atributos do UML)
        Lista<Entidades::Personagens::Inimigo> LIs; 

    public:
        ListaInimigos();
        ~ListaInimigos();

        void incluir(Entidades::Personagens::Inimigo* pI);
        void limpar();

        // Métodos de iteração
        void irParaPrimeiro();
        void irParaProximo();
        Entidades::Personagens::Inimigo* getAtual() const;
    };
}