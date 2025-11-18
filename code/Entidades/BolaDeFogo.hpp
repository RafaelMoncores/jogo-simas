#pragma once
#include "Entidade.hpp"
#include "Personagens/Jogador.hpp"

namespace Entidades
{
    class BolaDeFogo : public Entidade
    {
        private:
            sf::Texture textura;
            std::optional<sf::Sprite> shape;
            sf::Vector2f direcao;
            float velocidadeProjetil;
            int dano;
            bool ativo;

        public:
            BolaDeFogo(sf::Vector2f pos, sf::Vector2f dir, float vel = 600.f);
            ~BolaDeFogo();

            virtual void executar(float delta) override;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;

            void colidirComJogador(Personagens::Jogador* pJogador);

            bool getAtivo() const;
    };
}