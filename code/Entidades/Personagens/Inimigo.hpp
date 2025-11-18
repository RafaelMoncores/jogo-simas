#pragma once
#include "Personagem.hpp"
#include "Jogador.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Inimigo : public Personagem
        {
            protected:
                int nivel_maldade;

                Jogador* pJogador;
                Jogador* pJogador2;
                
                virtual Jogador* getJogadorMaisProximo();

            public:
                Inimigo(int vidas, sf::Vector2f pos, Jogador* pJ1, Jogador* pJ2);
                virtual ~Inimigo();

                virtual void danificar(Personagem* pOutro) = 0;
                virtual void executar(float delta) override = 0;
                virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override = 0;

                virtual void salvarDataBuffer() override;
                virtual void salvar() override;
        };
    }
}