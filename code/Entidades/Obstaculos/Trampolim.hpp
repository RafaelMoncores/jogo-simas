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
        class Trampolim : public Obstaculo
        {
            private:
                std::optional<sf::Sprite> sprite;
                sf::Texture textura;
                bool sobeDaEsquerda;
                float largura;
                sf::Vector2f m_pos;
                sf::Vector2f m_size;

            public:
                Trampolim(sf::Vector2f pos, sf::Vector2f size, bool sobeEsq, std::string caminhoTextura);
                ~Trampolim();

                virtual void desenhar() override;
                virtual sf::FloatRect getBoundingBox() const override;
                bool getSobeDaEsquerda() const;
                virtual void obstaculizar(Entidades::Personagens::Jogador* pJogador) override;
        };
    }
}