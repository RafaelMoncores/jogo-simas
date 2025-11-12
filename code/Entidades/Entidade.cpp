#include "Entidade.hpp"

namespace Entidades
{
    Entidade::Entidade() :
        Ente(),
        velocidade({0.0f, 0.0f}),
        G_ACCEL({0.0f, 800.f}),
        x(0),
        y(0),
        buffer(nullptr)
    {
    }

    Entidade::~Entidade()
    {
        // O corpo fica vazio, mas a definição DEVE existir
    }

    void Entidade::salvarDataBuffer()
    {
        // Método de persistência (ainda não implementado)
    }

    void Entidade::salvar()
    {
        // Método de persistência (ainda não implementado)
    }
}