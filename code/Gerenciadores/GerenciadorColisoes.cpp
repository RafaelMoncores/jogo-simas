#include "GerenciadorColisoes.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace Gerenciadores
{
    GerenciadorColisoes::GerenciadorColisoes() {}
    GerenciadorColisoes::~GerenciadorColisoes() {}

    void GerenciadorColisoes::verificarColisoes(
        Entidades::Personagens::Jogador* pJogador,
        std::list<Entidades::Obstaculos::Obstaculo*>* pObstaculos,
        std::list<Entidades::Personagens::Inimigo*>* pInimigos)
    {
        if (!pJogador) return;

        pJogador->setPodePular(false);
        for (auto* pObst : *pObstaculos)
        {
            tratarColisao(pJogador, pObst);
        }

        for (auto* pInim : *pInimigos)
        {
            pInim->setPodePular(false);
            for (auto* pObst : *pObstaculos)
            {
                tratarColisao(pInim, pObst);
            }
        }

        for (auto* pInim : *pInimigos)
        {
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
                pInim->perderVida();
            }
            else
            {
                pInim->danificar(pJogador);
            }
        }
    }
}