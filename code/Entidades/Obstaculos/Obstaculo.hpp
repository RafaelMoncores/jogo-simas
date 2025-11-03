#pragma once
#include "../Entidade.hpp"

namespace Entidades
{
    namespace Obstaculos
    {
        /*
        * Classe base abstrata para todos os obstáculos.
        * Ela herda os métodos virtuais puros de Entidade.
        * Implementa um 'executar' vazio, pois obstáculos são geralmente estáticos.
        * Força as classes filhas a implementar 'desenhar' e 'getBoundingBox'.
        */
        class Obstaculo : public Entidade
        {
        public:
            Obstaculo();
            virtual ~Obstaculo();

            // Implementação padrão vazia para obstáculos que não se movem
            virtual void executar(float delta) override;

            // Força classes filhas (como Plataforma) a implementar o desenhar
            virtual void desenhar() override = 0;

            // Força classes filhas (como Plataforma) a implementar a hitbox
            virtual sf::FloatRect getBoundingBox() const override = 0;
        };
    }
}