#pragma once
#include "Obstaculo.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <string>

namespace Entidades
{
    namespace Obstaculos
    {
        class Rampa : public Obstaculo
        {
        private:
            std::optional<sf::Sprite> sprite;
            sf::Texture textura;
            bool sobeDaEsquerda; // true = '/', false = '\'

        public:
            // Construtor agora recebe 'sobeEsq'
            Rampa(sf::Vector2f pos, sf::Vector2f size, bool sobeEsq, std::string caminhoTextura);
            ~Rampa();

            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;

            // Métodos para a colisão
            bool getSobeDaEsquerda() const;
        };
    }
}