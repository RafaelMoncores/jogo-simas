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

                    bool estaNaRampa;
                    bool rampaSobeEsquerda; // Para sabermos a direção do "slide"

                    void processarInputs(float delta);
                    virtual void aplicarFisica(float delta) override;

                public:
                    Jogador(sf::Vector2f pos);
                    ~Jogador();

                    virtual void executar(float delta) override;
                    virtual sf::FloatRect getBoundingBox() const override;
                    virtual void resolverColisao(Entidade* pOutra, sf::FloatRect boundsOutra) override;
            };
    }
}