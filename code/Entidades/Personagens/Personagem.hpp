#pragma once
#include "../Entidade.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Personagem : public Entidade
        {
            protected:
                int num_vidas;
            
            public:
                Personagem();
                virtual ~Personagem();

                virtual void executar(float delta) = 0;
                virtual void desenhar() = 0;
        };
    }
}