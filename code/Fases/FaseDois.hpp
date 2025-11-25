#pragma once
#include "Fase.hpp"

namespace Fases
{
    class FaseDois : public Fase
    {
        private:
            Entidades::Obstaculos::Plataforma* pChao_I;
            Entidades::Obstaculos::Plataforma* pChao_F;
            Entidades::Obstaculos::Plataforma* pChao_1;
            Entidades::Obstaculos::Plataforma* pChao_2;
            Entidades::Obstaculos::Plataforma* pChao_3;
        protected:
            virtual void criarObstaculos() override;
            virtual void criarInimigos() override;
            virtual void criarPlataformas() override;
            virtual void PosarInimigos() override;
            virtual void PosarObstaculos() override;
            virtual void criarChefao();
            virtual void criarObstDificil();
            virtual void criarInimigosFaceis();

        public:
            FaseDois(Jogo* pJogo, bool doisJogadores);
            FaseDois();
            ~FaseDois();

            const int maxChefoes;
    };
}