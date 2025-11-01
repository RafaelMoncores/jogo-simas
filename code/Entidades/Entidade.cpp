#include "Entidade.hpp"

namespace Entidades
{
    Entidade::Entidade() :
        Ente(),
        pCorpo(nullptr),
        velocidade({0.0f, 0.0f}),
        G_ACCEL({0.0f, 800.f})
    {
    }

    Entidade::~Entidade()
    {
        // O corpo fica vazio, mas a definição DEVE existir
    }
}