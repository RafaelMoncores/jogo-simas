#pragma once
#include "Obstaculo.hpp"
#include <SFML/Graphics.hpp>

namespace Entidades { namespace Personagens { class Jogador; } }

namespace Entidades
{
    namespace Obstaculos
    {
        class Parede : public Obstaculo
        {
            private:
                sf::RectangleShape corpo;
                sf::Texture textura;

            public:
                Parede(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura);
                Parede();
                ~Parede();

                virtual void executar(float delta) override;
                virtual void desenhar() override;
                virtual sf::FloatRect getBoundingBox() const override;
                virtual void obstaculizar(Entidades::Personagens::Jogador* pJogador) override;
                virtual void salvarDataBuffer() override;
        };
    }
}