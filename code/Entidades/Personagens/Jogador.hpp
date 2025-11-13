#pragma once
#include "Personagem.hpp"
// Obstaculo.hpp agora é incluído no .cpp

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

                void processarInputs(float delta);
                virtual void aplicarFisica(float delta) override;

            public:
                Jogador(sf::Vector2f pos);
                ~Jogador();

                bool getEstaAtacando() const;
                bool getCompletouFase() const;
                int getDirecao() const;

                virtual void executar(float delta) override;
                virtual sf::FloatRect getBoundingBox() const override;
                virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;
        };
    }
}