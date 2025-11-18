#pragma once
#include "Inimigo.hpp"
#include "Jogador.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Vampiro : public Inimigo
        {
        private:
            sf::Texture textura;
            
            float tamanho;
            float tempoTotal;
            int direcao;

            const float VELOCIDADE_PATRULHA;
            const float AMPLITUDE_SENOIDE;
            const float FREQUENCIA_SENOIDE;
            const float LIMITE_ESQUERDA;
            const float LIMITE_DIREITA;

            void processarAI(float delta);
            virtual void aplicarFisica(float delta) override;

        public:
            Vampiro(sf::Vector2f pos, float tamanho, Jogador* pJ1, Jogador* pJ2);
            ~Vampiro();

            virtual void executar(float delta) override;
            virtual void danificar(Personagem* pOutro) override;
            
            virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;
            
            virtual sf::FloatRect getBoundingBox() const override;

            virtual void salvarDataBuffer() override;
            virtual void salvar() override;
        };
    }
}