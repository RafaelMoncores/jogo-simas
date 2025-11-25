#include "ListaEntidades.hpp"

namespace Listas
{
    ListaEntidades::ListaEntidades() :
    LEs() {
    }

    ListaEntidades::~ListaEntidades() {
        // O destrutor de 'LEs' será chamado automaticamente
    }

    // Todos os métodos abaixo apenas repassam a ordem para a lista interna (Delegate Pattern)
    
    void ListaEntidades::incluir(Entidades::Entidade* pE) {
        LEs.incluir(pE);
    }

    void ListaEntidades::limpar() {
        LEs.limpar();
    }
    
    void ListaEntidades::remover(Entidades::Entidade* pE)
    {
        LEs.remover(pE);
    }

    void ListaEntidades::percorrer()
    {
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