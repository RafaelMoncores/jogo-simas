#pragma once
#include "../Entidade.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <algorithm>

namespace Entidades
{
    class Entidade; 

    namespace Personagens
    {
        class Personagem : public Entidade
        {
            protected:
                int num_vidas;

                std::optional<sf::Sprite> sprite;
                sf::Vector2f posInicial;

                const float VELOCIDADE_MAXIMA_LATERAL;
                const float ACELERACAO_LATERAL;
                const float FRICCAO_LATERAL;

                bool podePular;

                virtual void aplicarFisica(float delta);
                virtual void salvarDataBuffer() override;
                
            
            public:
                Personagem(int vidas, sf::Vector2f pos);
                virtual ~Personagem();

                void perderVida(int dano = 1);
                int getVidas() const;
                void setPosition(sf::Vector2f pos);

                virtual void desenhar() override;
                virtual sf::FloatRect getBoundingBox() const override;
                void setPodePular(bool pode);
                sf::Vector2f getVelocidade() const;

                virtual void fazerBounce(float forcaBounce);
                virtual void aplicarRepel(sf::Vector2f forcaRepel);

                virtual void salvar() override;

                virtual void executar(float delta) override = 0;
                virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) = 0;
        };
    }
}