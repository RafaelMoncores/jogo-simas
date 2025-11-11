#pragma once
#include "Obstaculo.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace Entidades { namespace Personagens { class Jogador; } }

namespace Entidades
{
    namespace Obstaculos
    {
        class Rampa : public Obstaculo
        {
            private:
                std::optional<sf::Sprite> sprite;
                sf::Texture textura;
                bool sobeDaEsquerda;
                float largura;

            public:
                Rampa(sf::Vector2f pos, sf::Vector2f size, bool sobeEsq, std::string caminhoTextura);
                ~Rampa();

                virtual void desenhar() override;
                virtual sf::FloatRect getBoundingBox() const override;
                bool getSobeDaEsquerda() const;
                virtual void obstaculizar(Personagens::Jogador* pJogador) override;
        };
    }
}