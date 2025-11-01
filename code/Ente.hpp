#pragma once
#include "GerenciadorGrafico.hpp"

class Ente
{
protected:
    static GerenciadorGrafico* pGG;

public:
    Ente();
    virtual ~Ente();

    static void setGerenciadorGrafico(GerenciadorGrafico* p);

    virtual void executar() = 0;
    virtual void desenhar() = 0;
};