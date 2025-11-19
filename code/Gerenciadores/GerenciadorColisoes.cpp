#include "GerenciadorColisoes.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include "../Listas/ListaObstaculos.hpp"
#include "../Listas/ListaInimigos.hpp"
#include "../Listas/ListaEntidades.hpp"
#include "../Entidades/BolaDeFogo.hpp"
#include "../Entidades/Personagens/Dragao.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace Gerenciadores
{
    GerenciadorColisoes::GerenciadorColisoes() {}
    GerenciadorColisoes::~GerenciadorColisoes() {}

    void GerenciadorColisoes::verificarColisoes(
        Entidades::Personagens::Jogador* pJogador,
        Listas::ListaObstaculos* pObstaculos,
        Listas::ListaInimigos* pInimigos,
        Listas::ListaEntidades* pEntidades,
        bool resetInimigos)
    {
        if (!pJogador) return;

        pJogador->setPodePular(false);

        // Se solicitado, resetamos a flag de pulo de todos inimigos uma vez
        // por frame. Isso evita que a segunda chamada (para o 2º jogador)
        // zere o estado depois que colisões com o primeiro jogador já o
        // tinham habilitado.
        if (resetInimigos)
        {
            for (pInimigos->irParaPrimeiro(); ; pInimigos->irParaProximo())
            {
                Entidades::Personagens::Inimigo* pInimInit = pInimigos->getAtual();
                if (pInimInit == NULL) break;
                pInimInit->setPodePular(false);
            }
        }

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

        for (pEntidades->irParaPrimeiro(); ; pEntidades->irParaProximo())
        {
            Entidades::Entidade* pE = pEntidades->getAtual();
            if (pE == NULL) break;

            Entidades::BolaDeFogo* pFogo = dynamic_cast<Entidades::BolaDeFogo*>(pE);
            
            if (pFogo)
            {
                sf::FloatRect boundsFogo = pFogo->getBoundingBox();

                if (pFogo->getPertenceAoJogador())
                {
                    for (pInimigos->irParaPrimeiro(); ; pInimigos->irParaProximo())
                    {
                        Entidades::Personagens::Inimigo* pInim = pInimigos->getAtual();
                        if (pInim == NULL) break;

                        if (dynamic_cast<Entidades::Personagens::Dragao*>(pInim))
                        {
                            sf::FloatRect boundsInimigo = pInim->getBoundingBox();
                            if (boundsFogo.findIntersection(boundsInimigo))
                            {
                                pInim->perderVida(pFogo->getDano());
                                pFogo->setAtivo(false);
                                pJogador->addPontos(100);
                                break;
                            }
                        }
                    }
                }
                else
                {
                    if (pJogador->getEstaAtacando())
                    {
                        sf::FloatRect boundsAtaque = pJogador->getHitboxAtaque();
                        if (boundsFogo.findIntersection(boundsAtaque))
                        {
                            pFogo->rebater();
                            continue; 
                        }
                    }

                    tratarColisao(pJogador, pFogo);
                }
            }
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Obstaculos::Obstaculo* pObst)
    {
        sf::FloatRect boundsJogador = pJogador->getBoundingBox();
        sf::FloatRect boundsObst = pObst->getBoundingBox();

        if (boundsJogador.findIntersection(boundsObst))
        {
            pObst->obstaculizar(pJogador);
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Inimigo* pInim, Entidades::Obstaculos::Obstaculo* pObst)
    {
        sf::FloatRect boundsInim = pInim->getBoundingBox();
        sf::FloatRect boundsObst = pObst->getBoundingBox();

        if (boundsInim.findIntersection(boundsObst))
        {
            pInim->colidir(pObst, boundsObst);
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::Personagens::Inimigo* pInim)
    {
        sf::FloatRect boundsJogador = pJogador->getBoundingBox();
        sf::FloatRect boundsInim = pInim->getBoundingBox();

        if (boundsJogador.findIntersection(boundsInim))
        {
            float velJogadorY = pJogador->getVelocidade().y;

            sf::Vector2f centroJogador(boundsJogador.position.x + boundsJogador.size.x / 2.f, boundsJogador.position.y + boundsJogador.size.y / 2.f);
            sf::Vector2f centroInimigo(boundsInim.position.x + boundsInim.size.x / 2.f, boundsInim.position.y + boundsInim.size.y / 2.f);
            sf::Vector2f distCentros(centroJogador.x - centroInimigo.x, centroJogador.y - centroInimigo.y);
            
            float somaMeiasLarguras = boundsJogador.size.x / 2.f + boundsInim.size.x / 2.f;
            float somaMeiasAlturas = boundsJogador.size.y / 2.f + boundsInim.size.y / 2.f;
            
            float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
            float overlapY = somaMeiasAlturas - std::abs(distCentros.y);
            
            if (velJogadorY > 0 && overlapY < overlapX && distCentros.y < 0)
            {
                pInim->perderVida(1);
                pJogador->fazerBounce(250.0f);
                pJogador->colidir(pInim, boundsInim);
            }
            else
            {
                pInim->danificar(pJogador);
            }
        }
    }

    void GerenciadorColisoes::tratarColisao(Entidades::Personagens::Jogador* pJogador, Entidades::BolaDeFogo* pFogo)
    {
        if (!pFogo->getAtivo()) return;

        sf::FloatRect boundsJogador = pJogador->getBoundingBox();
        sf::FloatRect boundsFogo = pFogo->getBoundingBox();

        if (boundsJogador.findIntersection(boundsFogo))
        {
            pFogo->colidirComJogador(pJogador);
        }
    }
}