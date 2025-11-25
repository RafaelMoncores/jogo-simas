#pragma once
#include "Obstaculo.hpp"
#include <SFML/Graphics.hpp>
#include <string>

namespace Entidades { namespace Personagens { class Jogador; } }

namespace Entidades
{
    namespace Obstaculos
    {
        class Plataforma : public Obstaculo
        {
        private:
            // Substituido o Sprite pelo RectangleShape para permitir repetição
            sf::RectangleShape corpo;
            std::string caminhoTextura;  // Armazena a textura para restauração
            
        public:
            Plataforma(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura);
            Plataforma(){};
            ~Plataforma();

            virtual void executar(float delta) override;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;
            virtual void obstaculizar(Entidades::Personagens::Jogador* pJogador) override;
            virtual void salvarDataBuffer() override;
        };
    }
}