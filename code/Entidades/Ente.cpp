#include "Ente.hpp"

namespace Entidades
{
    Gerenciadores::GerenciadorGrafico* Ente::pGG = nullptr;

    Ente::Ente() :
        pFig(nullptr)
    {
    }

    Ente::~Ente()
    {
    }

    void Ente::setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico* p)
    {
        pGG = p;
    }
}