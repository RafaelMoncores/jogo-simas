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
        class Plataforma : public Obstaculo
        {
        private:
            std::optional<sf::Sprite> sprite;
            sf::Texture textura;
            int altura;

        public:
            Plataforma(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura);
            ~Plataforma();

            virtual void executar(float delta) override;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;
            virtual void obstaculizar(Entidades::Personagens::Jogador* pJogador) override;
        };
    }
}