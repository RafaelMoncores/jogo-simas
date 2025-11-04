#pragma once
#include "Inimigo.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/Rampa.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Slime : public Inimigo
        {
        private:
            sf::Texture textura;
            
            Obstaculos::Plataforma* pPlataforma;
            float tempoEspera;
            float tempoTotal;
            int direcaoPulo;
            const float FORCA_PULO_INIMIGO;
            const float VELOCIDADE_LATERAL_INIMIGO;

            void processarAI(float delta);
            virtual void aplicarFisica(float delta) override;

        public:
            Slime(sf::Vector2f pos, Obstaculos::Plataforma* pPlat);
            ~Slime();

            virtual void executar(float delta) override;
            virtual void danificar(Personagem* pOutro) override;
            virtual void resolverColisao(Entidade* pOutra, sf::FloatRect boundsOutra) override;
        };
    }
}