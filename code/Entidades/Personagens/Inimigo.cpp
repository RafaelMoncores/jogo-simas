#include "Inimigo.hpp"

namespace Entidades
{
    namespace Personagens
    {
        Inimigo::Inimigo(int vidas, sf::Vector2f pos, Jogador* pJ1, Jogador* pJ2) :
            Personagem(vidas, pos),
            nivel_maldade(1),
            pJogador(pJ1),
            pJogador2(pJ2)
        {
        }

        Inimigo::~Inimigo()
        {
        }

        Entidades::Personagens::Jogador* Inimigo::getJogadorMaisProximo()
        {
            bool p1Valido = (pJogador != nullptr && pJogador->getVidas() > 0);
            bool p2Valido = (pJogador2 != nullptr && pJogador2->getVidas() > 0);

            if (p1Valido && !p2Valido) {
                return pJogador;
            }
            if (!p1Valido && p2Valido) {
                return pJogador2;
            }
            if (!p1Valido && !p2Valido) {
                return nullptr;
            }

            if (!sprite) return pJogador;

            sf::Vector2f posInimigo = sprite->getPosition();
            sf::Vector2f posP1 = pJogador->getBoundingBox().position;
            sf::Vector2f posP2 = pJogador2->getBoundingBox().position;
            
            float distP1 = (posInimigo.x - posP1.x)*(posInimigo.x - posP1.x) + (posInimigo.y - posP1.y)*(posInimigo.y - posP1.y);
            float distP2 = (posInimigo.x - posP2.x)*(posInimigo.x - posP2.x) + (posInimigo.y - posP2.y)*(posInimigo.y - posP2.y);

            if (distP1 < distP2) {
                return pJogador;
            } else {
                return pJogador2;
            }
        }

        void Inimigo::salvarDataBuffer()
        {
            Personagem::salvarDataBuffer();
        }

        void Inimigo::salvar()
        {
            Personagem::salvar();
        }
    }
}