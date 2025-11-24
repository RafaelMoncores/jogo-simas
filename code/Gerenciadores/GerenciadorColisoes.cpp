#include "GerenciadorColisoes.hpp"

// Inclusões necessárias para reconhecer os tipos completos
#include "../Entidades/Personagens/Inimigo.hpp"
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Obstaculo.hpp"
#include "../Entidades/BolaDeFogo.hpp" 
// Includes de inimigos específicos para pontuação
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

    // --- Métodos de Inclusão STL ---
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

    const bool GerenciadorColisoes::verificarColisao(Entidades::Entidade* pe1, Entidades::Entidade* pe2) const {
        if (!pe1 || !pe2) return false;
        sf::FloatRect bounds1 = pe1->getBoundingBox();
        sf::FloatRect bounds2 = pe2->getBoundingBox();
        
        return bounds1.findIntersection(bounds2).has_value();
    }

    void GerenciadorColisoes::executar(bool resetInimigos)
    {
        if (!pJog1) return;

        // Sempre resetamos o estado de pulo do jogador atual
        pJog1->setPodePular(false);

        // Reset apenas na primeira passagem por frame (quando solicitado).
        if (resetInimigos)
        {
            for (auto* inimigo : LIs) {
                if (inimigo) inimigo->setPodePular(false);
            }
        }

        // 1) Tratamento de colisões entre jogadores e obstáculos
        tratarColisoesJogsObstacs();
        // 2) Tratamento de colisões entre jogadores e inimigos
        tratarColisoesJogsInimgs();
        // 3) Tratamento de colisões envolvendo projéteis
        tratarColisoesJogsProjeteis();
    }

    void GerenciadorColisoes::tratarColisoesJogsObstacs()
    {
        for (auto* obstaculo : LOs) 
        {
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

            // 1. Inimigo vs Obstáculos
            for (auto* obstaculo : LOs)
            {
                if (verificarColisao(pInim, obstaculo)) {
                    sf::FloatRect boundsObst = obstaculo->getBoundingBox();
                    pInim->colidir(obstaculo, boundsObst);
                }
            }

            // 2. Jogador vs Inimigo
            if (verificarColisao(pJog1, pInim))
            {
                sf::FloatRect boundsJogador = pJog1->getBoundingBox();
                sf::FloatRect boundsInim = pInim->getBoundingBox();

                float velJogadorY = pJog1->getVelocidade().y;
                
                // Centro Jogador
                sf::Vector2f centroJogador(
                    boundsJogador.position.x + boundsJogador.size.x / 2.f, 
                    boundsJogador.position.y + boundsJogador.size.y / 2.f
                );
                
                // Centro Inimigo
                sf::Vector2f centroInimigo(
                    boundsInim.position.x + boundsInim.size.x / 2.f, 
                    boundsInim.position.y + boundsInim.size.y / 2.f
                );
                
                sf::Vector2f distCentros(centroJogador.x - centroInimigo.x, centroJogador.y - centroInimigo.y);
                
                // Meias Larguras/Alturas
                float somaMeiasLarguras = boundsJogador.size.x / 2.f + boundsInim.size.x / 2.f;
                float somaMeiasAlturas = boundsJogador.size.y / 2.f + boundsInim.size.y / 2.f;
                
                float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
                float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

                // Lógica de "Pular na cabeça"
                if (velJogadorY > 0 && overlapY < overlapX && distCentros.y < 0)
                {
                    pInim->perderVida(1);

                    if (pInim->getVidas() <= 0)
                    {
                        if (dynamic_cast<Entidades::Personagens::Gosma*>(pInim)) pJog1->addPontos(100);
                        else if (dynamic_cast<Entidades::Personagens::Vampiro*>(pInim)) pJog1->addPontos(300);
                        else if (dynamic_cast<Entidades::Personagens::Dragao*>(pInim)) pJog1->addPontos(500);
                    }

                    pJog1->fazerBounce(250.0f);
                    pJog1->colidir(pInim, boundsInim);
                }
                else
                {
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

            if (!pFogo->getPertenceAoJogador() && pJog1->getEstaAtacando())
            {
                sf::FloatRect boundsFogo = pFogo->getBoundingBox();
                sf::FloatRect boundsAtaque = pJog1->getHitboxAtaque();
                
                // Se o jogador rebateu o projétil inimigo com seu ataque, inverter
                // a direção e marcar como pertencente ao jogador.
                if (boundsFogo.findIntersection(boundsAtaque).has_value())
                {
                    pFogo->rebater();
                    // marca que o projétil agora pertence ao jogador que rebateu
                    if (pJog1) {
                        // assume-se que o gerenciador foi configurado para o jogador
                        // atual (setJogador) antes desta chamada
                        pFogo->setOwnerId(pJog1->getId());
                    }
                    continue;
                }
            }

            if (!pFogo->getPertenceAoJogador())
            {
                if (verificarColisao(pJog1, pFogo))
                {
                    pFogo->colidirComJogador(pJog1);
                }
            }
            else
            {
                // Projetil pertence ao jogador: checar colisão com inimigos
                sf::FloatRect boundsFogo = pFogo->getBoundingBox();
                for (auto* pInim : LIs)
                {
                    if (!pInim) continue;
                    sf::FloatRect boundsInim = pInim->getBoundingBox();
                    if (!boundsInim.findIntersection(boundsFogo).has_value()) continue;

                    // Acertou o inimigo: aplica dano e desativa o projetil
                    pInim->perderVida(pFogo->getDano());
                    pFogo->setAtivo(false);

                    // Caso especial: se o inimigo é um Dragao e foi atingido por
                    // um projetil do jogador, evitar que ele fique embutido em
                    // uma plataforma reposicionando-o imediatamente ao spawn.
                    if (auto* pDrag = dynamic_cast<Entidades::Personagens::Dragao*>(pInim))
                    {
                        pDrag->setPosition(pDrag->getPosInicial());
                        pDrag->aplicarRepel({0.f, 0.f});
                    }

                    if (pInim->getVidas() <= 0)
                    {
                        // Só credita pontos ao jogador dono do projetil
                        int owner = pFogo->getOwnerId();
                        if (pJog1 && owner == pJog1->getId()) {
                            if (dynamic_cast<Entidades::Personagens::Gosma*>(pInim)) pJog1->addPontos(100);
                            else if (dynamic_cast<Entidades::Personagens::Vampiro*>(pInim)) pJog1->addPontos(300);
                            else if (dynamic_cast<Entidades::Personagens::Dragao*>(pInim)) pJog1->addPontos(500);
                        }
                    }

                    break; // projetil consumido
                }
            }
        }
    }
}