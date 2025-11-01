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
    };
}