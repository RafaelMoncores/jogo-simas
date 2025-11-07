#include "ListaEntidades.hpp"

namespace Listas
{
    ListaEntidades::ListaEntidades() :
    LEs() {
        // O construtor de 'LEs' (Lista<Entidade>) é chamado aqui
    }

    ListaEntidades::~ListaEntidades() {
        // O destrutor de 'LEs' é chamado aqui (que chama limpar())
    }

    void ListaEntidades::incluir(Entidades::Entidade* pE) {
        // Delega a chamada para o objeto Lista interno
        LEs.incluir(pE);
    }

    void ListaEntidades::limpar() {
        LEs.limpar();
    }

    void ListaEntidades::irParaPrimeiro() {
        LEs.irParaPrimeiro();
    }

    void ListaEntidades::irParaProximo() {
        LEs.irParaProximo();
    }

    Entidades::Entidade* ListaEntidades::getAtual() const {
        return LEs.getAtual();
    }
}