#pragma once
#include "Lista.hpp"
#include "../Entidades/Entidade.hpp"

namespace Listas{
    class ListaEntidades {
    private:
        // Composição: A ListaEntidades TEM UMA Lista genérica de Entidades
        Lista<Entidades::Entidade> LEs;

    public:
        ListaEntidades();
        ~ListaEntidades();

        // Métodos de delegação (Facade)
        void incluir(Entidades::Entidade* pE);
        void limpar();
        void remover(Entidades::Entidade* pE);

        // Interface de iteração exposta
        void percorrer();
        void irParaPrimeiro();
        void irParaProximo();
        Entidades::Entidade* getAtual() const;
    };
}