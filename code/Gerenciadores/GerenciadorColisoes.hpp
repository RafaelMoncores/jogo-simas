#pragma once
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Entidades/Obstaculos/Obstaculo.hpp"
#include <list>

namespace Gerenciadores
{
    class GerenciadorColisoes
    {
    public:
        GerenciadorColisoes();
        ~GerenciadorColisoes();

        void verificarColisoes(
            Entidades::Personagens::Jogador* pJogador, 
            std::list<Entidades::Obstaculos::Obstaculo*>* pObstaculos // <-- DE Plataforma PARA Obstaculo
        );
    };
}