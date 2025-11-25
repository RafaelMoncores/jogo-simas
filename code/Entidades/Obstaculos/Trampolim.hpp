#pragma once
#include "Obstaculo.hpp"
#include <SFML/Graphics.hpp>

namespace Entidades { namespace Personagens { class Jogador; } }

namespace Entidades
{
    namespace Obstaculos
    {
        class Trampolim : public Obstaculo
        {
        private:
            sf::RectangleShape corpo;
            std::string caminhoTextura;  // Armazena a textura para restauração
            const float FORCA_REBOTE; 

        public:
            Trampolim(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura, float forca);
            Trampolim();
            ~Trampolim();

            virtual void executar(float delta) override;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;
            virtual void obstaculizar(Entidades::Personagens::Jogador* pJogador) override;
            virtual void salvarDataBuffer() override;
        };
    }
}