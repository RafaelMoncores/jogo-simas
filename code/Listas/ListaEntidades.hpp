#pragma once
#include "Lista.hpp"
#include "../Entidades/Entidade.hpp"

namespace Listas{
    class ListaEntidades {
    private:
        // Atributo 'LEs' (Lista de Entidades) do snippet
        Lista<Entidades::Entidade> LEs;

    public:
        
        ListaEntidades();
        ~ListaEntidades();

        // Métodos do snippet 
        void incluir(Entidades::Entidade* pE);
        void limpar();
        void remover(Entidades::Entidade* pE);

        // Métodos de iteração 
        void percorrer();
        void irParaPrimeiro();
        void irParaProximo();
        Entidades::Entidade* getAtual() const;
    };

}