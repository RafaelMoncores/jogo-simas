#pragma once
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Entidades/Obstaculos/Obstaculo.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
//#include <list>

namespace Listas 
{
    class ListaObstaculos;
    class ListaInimigos;
    class ListaEntidades;
}

namespace Entidades
{
    class BolaDeFogo;
}

namespace Gerenciadores
{
    class GerenciadorColisoes
    {
    private: 
        void tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Obstaculos::Obstaculo* pObst);
        void tratarColisao(Entidades::Personagens::Inimigo* pInim, Entidades::Obstaculos::Obstaculo* pObst);
        void tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Personagens::Inimigo* pInim);
        void tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::BolaDeFogo* pFogo);

    public:
        GerenciadorColisoes();
        ~GerenciadorColisoes();

        void verificarColisoes(
            Entidades::Personagens::Jogador* pJogador,
            Listas::ListaObstaculos* pObstaculos,
            Listas::ListaInimigos* pInimigos,
            Listas::ListaEntidades* pEntidades
        );
    };
}