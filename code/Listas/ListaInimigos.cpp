#include "ListaInimigos.hpp"
// Inclui a definição completa para a implementação
#include "../Entidades/Personagens/Inimigo.hpp" 

namespace Listas
{
    ListaInimigos::ListaInimigos() : LIs() {}
    ListaInimigos::~ListaInimigos() {} // ~Lista() já chama o limpar()

    void ListaInimigos::incluir(Entidades::Personagens::Inimigo* pI) {
        LIs.incluir(pI);
    }
    void ListaInimigos::limpar() {
        LIs.limpar();
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