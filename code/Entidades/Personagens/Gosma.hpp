#pragma once
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/Rampa.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Gosma : public Inimigo
        {
        private:
            sf::Texture textura;
            
            Obstaculos::Plataforma* pPlataforma;
            float tempoEspera;
            float tempoTotal;
            int direcaoPulo;
            const float FORCA_PULO_INIMIGO;
            const float VELOCIDADE_LATERAL_INIMIGO;

            Entidades::Personagens::Jogador* pJogador;
            const float raio_deteccao;

            void processarAI(float delta);
            virtual void aplicarFisica(float delta) override;

        public:
            Gosma(sf::Vector2f pos, Obstaculos::Plataforma* pPlat, Entidades::Personagens::Jogador* pJ);
            ~Gosma();

            virtual void executar(float delta) override;
            virtual void danificar(Personagem* pOutro) override;
            virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;

            virtual void salvarDataBuffer() override;
            virtual void salvar() override;
        };
    }
}