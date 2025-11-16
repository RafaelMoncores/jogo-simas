#pragma once

#include "Lista.hpp"

namespace Entidades { namespace Personagens { class Inimigo; } }

namespace Listas 
{
    class ListaInimigos {
    private:
        Lista<Entidades::Personagens::Inimigo> LIs; 

    public:
        ListaInimigos();
        ~ListaInimigos();

        void incluir(Entidades::Personagens::Inimigo* pI);
        void limpar();
        void remover(Entidades::Personagens::Inimigo* pI);

        void irParaPrimeiro();
        void irParaProximo();
        Entidades::Personagens::Inimigo* getAtual() const;
    };
}