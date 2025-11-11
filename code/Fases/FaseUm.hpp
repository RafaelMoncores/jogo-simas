// FaseUm.hpp
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
        public:
            FaseUm();
            ~FaseUm();
    };
}