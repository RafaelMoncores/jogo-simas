#pragma once
#include "Personagem.hpp"
#include "../Obstaculos/Obstaculo.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <algorithm>

namespace Entidades
{
    namespace Personagens
    {
        class Jogador : public Personagem
            {
                private:
                    sf::Texture textura;
                    std::optional<sf::Sprite> sprite;

                    const float VELOCIDADE_MAXIMA_LATERAL;
                    const float ACELERACAO_LATERAL;
                    const float FRICCAO_LATERAL;
                    const float FORCA_PULO;
                    const float MULTIPLICADOR_PULO_CURTO;

                    bool podePular;
                    sf::Vector2f posInicial;

                    void aplicarFisica(float delta);
                    void processarInputs(float delta);

                public:
                    Jogador(sf::Vector2f pos = {400.0f, 300.f});
                    ~Jogador();

                    virtual void executar(float delta) override;
                    virtual void desenhar() override;

                    virtual sf::FloatRect getBoundingBox() const override;
                    void setPodePular(bool pode);
                    void resolverColisao(Entidade* pOutra, sf::FloatRect boundsOutra);

                    void setPosition(sf::Vector2f pos);
            };
    }
}