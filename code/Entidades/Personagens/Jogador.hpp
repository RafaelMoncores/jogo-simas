#pragma once
#include "Personagem.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Jogador : public Personagem
        {
            private:
                sf::Texture textura;
                const float FORCA_PULO;
                const float MULTIPLICADOR_PULO_CURTO;

                int direcao;
                bool estaAtacando;
                bool completouFase;
                float tempoAtaque;
                const float COOLDOWN_ATAQUE;

                float pontos;

                void processarInputs(float delta);
                virtual void aplicarFisica(float delta) override;

            public:
                Jogador(sf::Vector2f pos);
                ~Jogador();

                bool getEstaAtacando() const;
                bool getCompletouFase() const;
                int getDirecao() const;

                int getPontos() const;
                sf::FloatRect getHitboxAtaque() const;
                void addPontos(int p);
                void setPontos(float p);

                virtual void executar(float delta) override;
                virtual sf::FloatRect getBoundingBox() const override;
                virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;
        };
    }
}