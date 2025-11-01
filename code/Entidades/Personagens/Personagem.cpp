#include "Personagem.hpp"

namespace Entidades
{
    namespace Personagens
    {
        Personagem::Personagem() :
            Entidade(),
            num_vidas(3)
        {
        }

        Personagem::~Personagem()
        {
        }
    }
}