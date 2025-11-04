#include "Personagem.hpp"

namespace Entidades
{
    namespace Personagens
    {
        Personagem::Personagem(int vidas) :
            Entidade(),
            num_vidas(vidas)
        {
        }

        Personagem::~Personagem()
        {
        }

        void Personagem::perderVida()
        {
            num_vidas--;
            if (num_vidas < 0)
            {
                num_vidas = 0;
            }
            // (Aqui, futuramente, se num_vidas == 0, "desativar" o personagem)
        }

        int Personagem::getVidas() const
        {
            return num_vidas;
        }
    }
}