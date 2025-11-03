#pragma once
#include "Ente.hpp"
#include <SFML/Graphics.hpp>

namespace Entidades
{
    class Entidade : public Ente
    {
        protected:
            sf::Sprite* pCorpo;
            sf::Vector2f velocidade;
            
            const sf::Vector2f G_ACCEL;

        public:
            Entidade();
            virtual ~Entidade();

            // 1. Lógica da entidade (movimento, IA, etc.)
            virtual void executar(float delta) = 0;

            // 2. Como a entidade se desenha
            virtual void desenhar() = 0;

            // 3. A "hitbox" da entidade
            virtual sf::FloatRect getBoundingBox() const = 0;
    };
}