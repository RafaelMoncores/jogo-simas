#pragma once
#include "Entidade.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <algorithm>

class Jogador : public Entidade
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

        void aplicarFisica(float delta);
        void processarInputs(float delta);

    public:
        Jogador();
        ~Jogador();

        virtual void executar(float delta) override;
        virtual void desenhar() override;
};