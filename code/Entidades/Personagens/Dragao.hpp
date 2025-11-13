#pragma once
#include "Inimigo.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Dragao : public Inimigo
        {
        private:
            sf::Texture textura;
            short int forca; 

            virtual void aplicarFisica(float delta) override;

        public:
            Dragao(sf::Vector2f pos);
            ~Dragao();

            virtual void executar(float delta) override;
            virtual void danificar(Personagem* pOutro) override;
            virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;

            virtual void salvarDataBuffer() override;
            virtual void salvar() override;
        };
    }
}