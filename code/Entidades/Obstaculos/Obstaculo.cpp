#include "Obstaculo.hpp"

namespace Entidades
{
    namespace Obstaculos
    {
        Obstaculo::Obstaculo() :
            Entidade(),
            danoso(false)
        {
        }

        Obstaculo::~Obstaculo()
        {
        }

        void Obstaculo::executar(float delta)
        {
            // não faz nada
        }
    }
}