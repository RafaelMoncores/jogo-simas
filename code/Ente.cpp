#include "Ente.hpp"

GerenciadorGrafico* Ente::pGG = nullptr;

Ente::Ente()
{
}

Ente::~Ente()
{
}

void Ente::setGerenciadorGrafico(GerenciadorGrafico* p)
{
    pGG = p;
}