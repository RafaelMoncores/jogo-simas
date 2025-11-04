#include "Inimigo.hpp"

namespace Entidades
{
    namespace Personagens
    {
        Inimigo::Inimigo(int vidas, sf::Vector2f pos) :
            Personagem(vidas, pos),
            nivel_maldade(1)
        {
        }

        Inimigo::~Inimigo()
        {
        }
    }
}