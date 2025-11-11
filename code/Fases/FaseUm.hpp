#pragma once
#include "Fase.hpp"

namespace Fases
{
    class FaseUm : public Fase
    {
        private:
            Entidades::Obstaculos::Plataforma* pChao2_slime;
            Entidades::Obstaculos::Plataforma* pPlat1_slime;
            Entidades::Obstaculos::Plataforma* pPlat4_slime;
        protected:
            virtual void criarObstaculos() override;
            virtual void criarInimigos() override;
            virtual void criarPlataformas() override;
            virtual void PosarInimigos() override;
            virtual void PosarObstaculos() override;
            virtual void criarInimMedios();
            virtual void criarObstMedios();

        public:
            FaseUm();
            ~FaseUm();

            const int maxInimMedios;
    };
}