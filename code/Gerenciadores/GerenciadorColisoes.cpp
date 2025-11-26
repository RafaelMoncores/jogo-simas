#include "GerenciadorColisoes.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Obstaculo.hpp"
#include "../Entidades/BolaDeFogo.hpp" 
#include "../Entidades/Personagens/Dragao.hpp"
#include "../Entidades/Personagens/Gosma.hpp"
#include "../Entidades/Personagens/Vampiro.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <algorithm> 
#include <cmath>

namespace Gerenciadores
{
    GerenciadorColisoes::GerenciadorColisoes() : 
        LIs(), LOs(), LPs(), pJog1(nullptr) 
    {
    }

    GerenciadorColisoes::~GerenciadorColisoes() 
    {
        limpar();
    }

    // --- Métodos de preenchimento das listas de física ---
    void GerenciadorColisoes::incluirInimigo(Entidades::Personagens::Inimigo* pi) {
        if (pi) LIs.push_back(pi); 
    }

    void GerenciadorColisoes::incluirObstaculo(Entidades::Obstaculos::Obstaculo* po) {
        if (po) LOs.push_back(po); 
    }

    void GerenciadorColisoes::incluirProjetil(Entidades::BolaDeFogo* pj) {
        if (pj) LPs.insert(pj); 
    }

    void GerenciadorColisoes::setJogador(Entidades::Personagens::Jogador* pJ) {
        pJog1 = pJ;
    }

    void GerenciadorColisoes::limpar() {
        LIs.clear();
        LOs.clear();
        LPs.clear();
        pJog1 = nullptr;
    }

    // Algoritmo AABB (Axis-Aligned Bounding Box) padrão do SFML
    const bool GerenciadorColisoes::verificarColisao(Entidades::Entidade* pe1, Entidades::Entidade* pe2) const {
        if (!pe1 || !pe2) return false;
        sf::FloatRect bounds1 = pe1->getBoundingBox();
        sf::FloatRect bounds2 = pe2->getBoundingBox();
        
        return bounds1.findIntersection(bounds2).has_value();
    }

    // Loop principal da física: reseta estados e verifica todas as interações
    void GerenciadorColisoes::executar(bool resetInimigos)
    {
        if (!pJog1) return;

        pJog1->setPodePular(false);

        // Otimização: reseta inimigos apenas uma vez por frame
        if (resetInimigos)
        {
            for (auto* inimigo : LIs) {
                if (inimigo) inimigo->setPodePular(false);
            }
        }

        tratarColisoesJogsObstacs();
        tratarColisoesJogsInimgs();
        tratarColisoesJogsProjeteis();
    }

    void GerenciadorColisoes::tratarColisoesJogsObstacs()
    {
        for (auto* obstaculo : LOs) 
        {
            // Se colidir, delega para o obstáculo decidir como bloquear o jogador (Visitor Pattern)
            if (verificarColisao(pJog1, obstaculo)) {
                obstaculo->obstaculizar(pJog1);
            }
        }
    }

    void GerenciadorColisoes::tratarColisoesJogsInimgs()
    {
        for (auto* pInim : LIs)
        {
            if (!pInim) continue;

            // Física Inimigo vs Cenário (para eles não caírem do chão)
            for (auto* obstaculo : LOs)
            {
                if (verificarColisao(pInim, obstaculo)) {
                    sf::FloatRect boundsObst = obstaculo->getBoundingBox();
                    pInim->colidir(obstaculo, boundsObst);
                }
            }

            // Lógica de Combate: Jogador vs Inimigo
            if (verificarColisao(pJog1, pInim))
            {
                sf::FloatRect boundsJogador = pJog1->getBoundingBox();
                sf::FloatRect boundsInim = pInim->getBoundingBox();

                // Cálculos vetoriais para determinar o centro e distâncias
                sf::Vector2f centroJogador(
                    boundsJogador.position.x + boundsJogador.size.x / 2.f, 
                    boundsJogador.position.y + boundsJogador.size.y / 2.f
                );
                sf::Vector2f centroInimigo(
                    boundsInim.position.x + boundsInim.size.x / 2.f, 
                    boundsInim.position.y + boundsInim.size.y / 2.f
                );
                
                sf::Vector2f distCentros(centroJogador.x - centroInimigo.x, centroJogador.y - centroInimigo.y);
                float overlapX = (boundsJogador.size.x / 2.f + boundsInim.size.x / 2.f) - std::abs(distCentros.x);
                float overlapY = (boundsJogador.size.y / 2.f + boundsInim.size.y / 2.f) - std::abs(distCentros.y);

                // Detecta se o jogador está caindo em cima do inimigo (Mecânica estilo Mario)
                if (pJog1->getVelocidade().y > 0 && overlapY < overlapX && distCentros.y < 0)
                {
                    pInim->perderVida(1);

                    // Sistema de pontuação 
                    if (pInim->getVidas() <= 0)
                    {
                        if (dynamic_cast<Entidades::Personagens::Gosma*>(pInim)) pJog1->addPontos(100);
                        else if (dynamic_cast<Entidades::Personagens::Vampiro*>(pInim)) pJog1->addPontos(300);
                        else if (dynamic_cast<Entidades::Personagens::Dragao*>(pInim)) pJog1->addPontos(500);
                    }

                    pJog1->fazerBounce(250.0f); // Impulso para cima
                    pJog1->colidir(pInim, boundsInim); // Resolve a física para não atravessar
                }
                else
                {
                    // Colisão lateral ou inferior causa dano ao jogador
                    pInim->danificar(pJog1);
                }
            }
        }
    }

    void GerenciadorColisoes::tratarColisoesJogsProjeteis()
    {
        for (auto* pFogo : LPs)
        {
            if (!pFogo || !pFogo->getAtivo()) continue;

            // Mecânica de Rebater (Parry): Se atacar o projétil inimigo, ele vira seu
            if (!pFogo->getPertenceAoJogador() && pJog1->getEstaAtacando())
            {
                sf::FloatRect boundsFogo = pFogo->getBoundingBox();
                sf::FloatRect boundsAtaque = pJog1->getHitboxAtaque();
                
                if (boundsFogo.findIntersection(boundsAtaque).has_value())
                {
                    pFogo->rebater();
                    if (pJog1) pFogo->setOwnerId(pJog1->getId());
                    continue;
                }
            }

            // Colisão Jogador vs Projétil Inimigo
            if (!pFogo->getPertenceAoJogador())
            {
                if (verificarColisao(pJog1, pFogo))
                {
                    pFogo->colidirComJogador(pJog1);
                }
            }
            // Colisão Projétil Jogador vs Inimigos
            else
            {
                sf::FloatRect boundsFogo = pFogo->getBoundingBox();
                for (auto* pInim : LIs)
                {
                    if (!pInim) continue;
                    sf::FloatRect boundsInim = pInim->getBoundingBox();
                    if (!boundsInim.findIntersection(boundsFogo).has_value()) continue;

                    pInim->perderVida(pFogo->getDano());
                    pFogo->setAtivo(false); // Destrói o projétil

                    //Evita que Dragão fique preso em plataforma ao ser atingido
                    if (auto* pDrag = dynamic_cast<Entidades::Personagens::Dragao*>(pInim))
                    {
                        pDrag->setPosition(pDrag->getPosInicial());
                        pDrag->aplicarRepel({0.f, 0.f});
                    }

                    // Pontuação por morte via projétil
                    if (pInim->getVidas() <= 0 && pJog1 && pFogo->getOwnerId() == pJog1->getId())
                    {
                        if (dynamic_cast<Entidades::Personagens::Gosma*>(pInim)) pJog1->addPontos(100);
                        else if (dynamic_cast<Entidades::Personagens::Vampiro*>(pInim)) pJog1->addPontos(300);
                        else if (dynamic_cast<Entidades::Personagens::Dragao*>(pInim)) pJog1->addPontos(500);
                    }
                    break; 
                }
            }
        }
    }
}