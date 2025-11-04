#pragma once
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Entidades/Obstaculos/Obstaculo.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include <list>

namespace Gerenciadores
{
    class GerenciadorColisoes
    {
    private: 
        void tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Obstaculos::Obstaculo* pObst);
        void tratarColisao(Entidades::Personagens::Inimigo* pInim, Entidades::Obstaculos::Obstaculo* pObst);
        void tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Personagens::Inimigo* pInim);

    public:
        GerenciadorColisoes();
        ~GerenciadorColisoes();

        void verificarColisoes(
            Entidades::Personagens::Jogador* pJogador, 
            std::list<Entidades::Obstaculos::Obstaculo*>* pObstaculos,
            std::list<Entidades::Personagens::Inimigo*>* pInimigos
        );
    };
}