#pragma once
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/Trampolim.hpp"

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

            const float raio_deteccao;

            void processarAI(float delta);
            virtual void aplicarFisica(float delta) override;

        public:
            Gosma(sf::Vector2f pos ={0.f,0.f}, Obstaculos::Plataforma* pPlat = nullptr, Jogador* pJ1 = nullptr, Jogador* pJ2 = nullptr);
            ~Gosma();

            virtual void executar(float delta) override;
            virtual void danificar(Personagem* pOutro) override;
            virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;

            virtual sf::FloatRect getBoundingBox() const override;

            virtual void salvarDataBuffer() override;
            virtual void salvar() override;
        };
    }
}