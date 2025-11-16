#include "ListaInimigos.hpp"
#include "../Entidades/Personagens/Inimigo.hpp" 

namespace Listas
{
    ListaInimigos::ListaInimigos() : LIs() {}
    ListaInimigos::~ListaInimigos() {}

    void ListaInimigos::incluir(Entidades::Personagens::Inimigo* pI) {
        LIs.incluir(pI);
    }
    void ListaInimigos::limpar() {
        LIs.limpar();
    }

    void ListaInimigos::remover(Entidades::Personagens::Inimigo* pI)
    {
        LIs.remover(pI);
    }

    void ListaInimigos::irParaPrimeiro() {
        LIs.irParaPrimeiro();
    }
    void ListaInimigos::irParaProximo() {
        LIs.irParaProximo();
    }
    Entidades::Personagens::Inimigo* ListaInimigos::getAtual() const {
        return LIs.getAtual();
    }
}