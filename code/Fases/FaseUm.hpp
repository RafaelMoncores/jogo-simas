#pragma once
#include "Fase.hpp"

namespace Fases
{
    class FaseUm : public Fase
    {
        private:
            // Ponteiros auxiliares para spawnar inimigos em cima de plataformas específicas
            Entidades::Obstaculos::Plataforma* pChao2_Gosma;
            Entidades::Obstaculos::Plataforma* pPlat1_Gosma;
            Entidades::Obstaculos::Plataforma* pPlat4_Gosma;
        
        protected:
            // Implementação dos métodos de criação procedural
            virtual void criarObstaculos() override;
            virtual void criarInimigos() override;
            virtual void criarPlataformas() override;
            virtual void PosarInimigos() override; // Reservado para load
            virtual void PosarObstaculos() override; // Reservado para load
            
            // Spawners específicos desta fase
            virtual void criarInimMedios();
            virtual void criarObstMedios();
            virtual void criarInimigosFaceis();

        public:
            FaseUm(Jogo* pJogo, bool doisJogadores);
            FaseUm();
            ~FaseUm();

            const int maxInimMedios;
    };
}