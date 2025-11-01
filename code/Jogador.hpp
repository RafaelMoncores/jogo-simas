#pragma once
#include "Entidade.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class Jogador : public Entidade
{
    private:
        sf::Texture textura;
        std::optional<sf::Sprite> sprite;

        // --- Variáveis de Movimento ---
        const float VELOCIDADE_LATERAL;
        const float FORCA_PULO;
        bool podePular;

        void aplicarGravidade();
        void processarInputs();

    public:
        Jogador();
        ~Jogador();

        virtual void executar() override;
        virtual void desenhar() override;
};