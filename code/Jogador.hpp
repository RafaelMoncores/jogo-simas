#pragma once
#include "Entidade.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class Jogador : public Entidade
{
    private:
        sf::Texture textura;
        std::optional<sf::Sprite> sprite;

        void aplicarGravidade();

    public:
        Jogador();
        ~Jogador();

        virtual void executar() override;
        virtual void desenhar() override;
};