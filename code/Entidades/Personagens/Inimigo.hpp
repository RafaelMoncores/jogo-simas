#pragma once
#include "Personagem.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Inimigo : public Personagem
        {
        protected:
            int nivel_maldade;

        public:
            Inimigo(int vidas, sf::Vector2f pos);
            virtual ~Inimigo();

            virtual void danificar(Personagem* pOutro) = 0;
            virtual void executar(float delta) override = 0;
            virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override = 0;

            virtual void salvarDataBuffer() override;
            virtual void salvar() override;
        };
    }
}