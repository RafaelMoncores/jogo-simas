#include "ListaObstaculos.hpp"
// Inclui a definição completa para a implementação
#include "../Entidades/Obstaculos/Obstaculo.hpp" 

namespace Listas
{
    ListaObstaculos::ListaObstaculos() : LOs() {}
    ListaObstaculos::~ListaObstaculos() {} // ~Lista() já chama o limpar()

    void ListaObstaculos::incluir(Entidades::Obstaculos::Obstaculo* pO) {
        LOs.incluir(pO);
    }
    void ListaObstaculos::limpar() {
        LOs.limpar();
    }
    void ListaObstaculos::irParaPrimeiro() {
        LOs.irParaPrimeiro();
    }
    void ListaObstaculos::irParaProximo() {
        LOs.irParaProximo();
    }
    Entidades::Obstaculos::Obstaculo* ListaObstaculos::getAtual() const {
        return LOs.getAtual();
    }
}