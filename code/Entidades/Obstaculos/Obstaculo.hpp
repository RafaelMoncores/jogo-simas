#pragma once
#include "../Entidade.hpp"

namespace Entidades
{
    namespace Obstaculos
    {
        class Obstaculo : public Entidade
        {
            protected:
                bool danoso;
            public:
                Obstaculo();
                virtual ~Obstaculo();

                virtual void executar(float delta) override;
                virtual void desenhar() override = 0;
                virtual sf::FloatRect getBoundingBox() const override = 0;
                virtual void obstaculizar(Personagens::Jogador* pJogador) = 0;
        };
    }
}