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
                Personagem(int vidas = 3);
                virtual ~Personagem();

                void perderVida();
                int getVidas() const;

                virtual void executar(float delta) = 0;
                virtual void desenhar() = 0;
        };
    }
}