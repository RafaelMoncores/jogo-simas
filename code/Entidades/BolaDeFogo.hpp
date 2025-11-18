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
            bool pertenceAoJogador;

        public:
            BolaDeFogo(sf::Vector2f pos, sf::Vector2f dir, float vel = 600.f, bool doJogador = false);
            ~BolaDeFogo();

            virtual void executar(float delta) override;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;

            void colidirComJogador(Personagens::Jogador* pJogador);

            bool getAtivo() const;
            void setAtivo(bool af);
            void rebater();
            bool getPertenceAoJogador() const;
            int getDano() const;
    };
}