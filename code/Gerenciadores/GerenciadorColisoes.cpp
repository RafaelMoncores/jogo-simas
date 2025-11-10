#include "GerenciadorColisoes.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include "../Listas/ListaObstaculos.hpp"
#include "../Listas/ListaInimigos.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace Gerenciadores
{
    GerenciadorColisoes::GerenciadorColisoes() {}
    GerenciadorColisoes::~GerenciadorColisoes() {}

    void GerenciadorColisoes::verificarColisoes(
        Entidades::Personagens::Jogador* pJogador,
        Listas::ListaObstaculos* pObstaculos,
        Listas::ListaInimigos* pInimigos)
    {
        if (!pJogador) return;

        pJogador->setPodePular(false);
        for (pObstaculos->irParaPrimeiro(); ; pObstaculos->irParaProximo())
        {
            Entidades::Obstaculos::Obstaculo* pObst = pObstaculos->getAtual();
            if (pObst == NULL) break;
            tratarColisao(pJogador, pObst);
        }

        for (pInimigos->irParaPrimeiro(); ; pInimigos->irParaProximo())
        {
            Entidades::Personagens::Inimigo* pInim = pInimigos->getAtual();
            if (pInim == NULL) break;

            pInim->setPodePular(false);
            
            // Loop C++03 aninhado
            for (pObstaculos->irParaPrimeiro(); ; pObstaculos->irParaProximo())
            {
                Entidades::Obstaculos::Obstaculo* pObst = pObstaculos->getAtual();
                if (pObst == NULL) break;
                tratarColisao(pInim, pObst);
            }
        }

        for (pInimigos->irParaPrimeiro(); ; pInimigos->irParaProximo())
        {
            Entidades::Personagens::Inimigo* pInim = pInimigos->getAtual();
            if (pInim == NULL) break;
            tratarColisao(pJogador, pInim);
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Obstaculos::Obstaculo* pObst)
    {
        sf::FloatRect boundsJogador = pJogador->getBoundingBox();
        sf::FloatRect boundsObst = pObst->getBoundingBox();

        if (boundsJogador.findIntersection(boundsObst))
        {
            pJogador->resolverColisao(pObst, boundsObst);
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Inimigo* pInim, Entidades::Obstaculos::Obstaculo* pObst)
    {
        sf::FloatRect boundsInim = pInim->getBoundingBox();
        sf::FloatRect boundsObst = pObst->getBoundingBox();

        if (boundsInim.findIntersection(boundsObst))
        {
            pInim->resolverColisao(pObst, boundsObst);
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Personagens::Inimigo* pInim)
    {
        sf::FloatRect boundsJogador = pJogador->getBoundingBox();
        sf::FloatRect boundsInim = pInim->getBoundingBox();

        if (boundsJogador.findIntersection(boundsInim))
        {
            float peJogador = boundsJogador.position.y + boundsJogador.size.y;
            float topoInimigo = boundsInim.position.y;
            float velJogadorY = pJogador->getVelocidade().y;

            if (velJogadorY > 0 && (peJogador - velJogadorY * 0.016f) < topoInimigo)
            {
                pInim->perderVida(1);
            }
            else if (pJogador->getEstaAtacando())
            {
                float centroJogadorX = boundsJogador.position.x + boundsJogador.size.x / 2.f;
                float centroInimigoX = boundsInim.position.x + boundsInim.size.x / 2.f;
                float distH = centroInimigoX - centroJogadorX;
                int dirJ = pJogador->getDirecao();

                bool olhandoParaInimigo = (dirJ > 0 && distH > 0) || (dirJ < 0 && distH < 0);

                if (olhandoParaInimigo)
                {
                    pInim->perderVida(1);
                }
                else
                {
                    pInim->danificar(pJogador);
                }
            }
            else
            {
                pInim->danificar(pJogador);
            }
        }
    }
}