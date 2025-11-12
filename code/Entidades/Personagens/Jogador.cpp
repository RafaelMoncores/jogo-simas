#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/Trampolim.hpp"
#include <iostream>
#include <stdlib.h>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>

namespace Entidades
{
    namespace Personagens
    {

        Jogador::Jogador(sf::Vector2f pos) :
            Personagem(3, pos),
            FORCA_PULO(420.0f),
            MULTIPLICADOR_PULO_CURTO(3.0f),
            estaNaTrampolim(false),
            rampaSobeEsquerda(false),
            direcao(1),
            estaAtacando(false),
            tempoAtaque(0.0f),
            COOLDOWN_ATAQUE(0.3f)
        {
            if (!textura.loadFromFile("player.png"))
            {
                std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
                exit(1);
            }
            
            sprite.emplace(textura); 
            sprite->setPosition(posInicial);
        }

        Jogador::~Jogador()
        {
        }

        void Jogador::processarInputs(float delta)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && podePular)
            {
                velocidade.y = -FORCA_PULO;
                podePular = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !estaAtacando)
            {
                estaAtacando = true;
                tempoAtaque = COOLDOWN_ATAQUE;
                // (TODO: mudar para a animação do sprite para "atacando")
            }
        }

        void Jogador::aplicarFisica(float delta)
        {
            const float FATOR_LENTIDAO_RAMPA = 0.30f;
            float modificadorAceleracao = 0.7f; 

            bool inputEsquerda = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
            bool inputDireita = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
            bool noInputHorizontal = !inputEsquerda && !inputDireita;

            if (estaNaTrampolim)
            {
                if ((rampaSobeEsquerda && inputDireita) ||
                    (!rampaSobeEsquerda && inputEsquerda))
                {
                    modificadorAceleracao = FATOR_LENTIDAO_RAMPA;
                }
            }

            if (inputEsquerda)
            {
                velocidade.x -= ACELERACAO_LATERAL * modificadorAceleracao * delta;
                direcao = -1;
            }
            if (inputDireita)
            {
                velocidade.x += ACELERACAO_LATERAL * modificadorAceleracao * delta;
                direcao = 1;
            }

            if (velocidade.y < 0.0f && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                velocidade.y += G_ACCEL.y * MULTIPLICADOR_PULO_CURTO * delta;
            }

            velocidade.y += G_ACCEL.y * delta;

            if (estaNaTrampolim) 
            {
                // ESTAMOS NA RAMPA
                const float FORCA_SLIDE_RAMPA = 10.0f; // O seu valor de slide

                // SÓ APLIQUE O SLIDE SE O JOGADOR ESTIVER PARADO
                if (noInputHorizontal)
                {
                    // Trampolim '/' (sobe da esquerda) -> desliza para a ESQUERDA (negativo)
                    if (rampaSobeEsquerda) 
                    {
                        velocidade.x -= FORCA_SLIDE_RAMPA * delta; // <--- CORRIGIDO
                    }
                    // Trampolim '\' (sobe da direita) -> desliza para a DIREITA (positivo)
                    else 
                    {
                        velocidade.x += FORCA_SLIDE_RAMPA * delta; // <--- CORRIGIDO
                    }
                }
            }
            else if (podePular && noInputHorizontal)
            {
                // ESTAMOS EM CHÃO PLANO (lógica de fricção original)
                if (std::abs(velocidade.x) > 0.1f)
                {
                    if (velocidade.x > 0.0f)
                        velocidade.x -= FRICCAO_LATERAL * delta;
                    else
                        velocidade.x += FRICCAO_LATERAL * delta;
                }
                if (std::abs(velocidade.x) < 0.5f)
                {
                    velocidade.x = 0.0f;
                }
            }

            // 5. LIMITAR VELOCIDADE E MOVER (Sem alteração)
            velocidade.x = std::clamp(velocidade.x, -VELOCIDADE_MAXIMA_LATERAL, VELOCIDADE_MAXIMA_LATERAL);
            
            if (sprite)
            {
                sprite->move(velocidade * delta);
                
                float deathPlaneY = 1500.f;
                if (sprite->getPosition().y > deathPlaneY)
                {
                    perderVida(num_vidas);
                }
            }
        }

        void Jogador::executar(float delta)
        {
            if (estaAtacando)
            {
                tempoAtaque -= delta;
                if (tempoAtaque <= 0.0f)
                {
                    estaAtacando = false;
                    // (TODO: Mudar animação de volta para "idle" ou "walk")
                }
            }
            processarInputs(delta);
            aplicarFisica(delta);
        }

        bool Jogador::getEstaAtacando() const
        {
            return estaAtacando;
        }

        int Jogador::getDirecao() const
        {
            return direcao;
        }

        sf::FloatRect Jogador::getBoundingBox() const
        {
            if (sprite)
            {
                sf::FloatRect bounds = sprite->getGlobalBounds();
                
                float ajusteLargura = 48.0f;
                bounds.position.x += ajusteLargura / 2.f;
                bounds.size.x -= ajusteLargura;

                return bounds;
            }
            return {};
        }

        void Jogador::colidir(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            if (!sprite) return;
            sf::FloatRect boundsPropria = getBoundingBox();


            sf::Vector2f posAtual = sprite->getPosition();
            sf::Vector2f centroProprio(boundsPropria.position.x + boundsPropria.size.x / 2.f, boundsPropria.position.y + boundsPropria.size.y / 2.f);

            sf::Vector2f centroOutra(boundsOutra.position.x + boundsOutra.size.x / 2.f, boundsOutra.position.y + boundsOutra.size.y / 2.f);
            sf::Vector2f distCentros(centroProprio.x - centroOutra.x, centroProprio.y - centroOutra.y);

            float somaMeiasLarguras = boundsPropria.size.x / 2.f + boundsOutra.size.x / 2.f;
            float somaMeiasAlturas = boundsPropria.size.y / 2.f + boundsOutra.size.y / 2.f;
            float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
            float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

            if (overlapX <= 0 || overlapY <= 0) return;

            if (auto* pTrampolim = dynamic_cast<Obstaculos::Trampolim*>(pOutra))
            {
                // --- INÍCIO DA LÓGICA DA RAMPA ---
                
                // Função interna para calcular a altura Y exata da rampa em um ponto X
                auto calcularAlturaTrampolim = [&](float pontoX) -> float {
                    float xRelativo = pontoX - boundsOutra.position.x;
                    float pctTrampolim = std::clamp(xRelativo / boundsOutra.size.x, 0.f, 1.f);
                    float alturaNoPonto;

                    if (pTrampolim->getSobeDaEsquerda()) {
                        // Trampolim '/'
                        alturaNoPonto = boundsOutra.size.y * (1.f - pctTrampolim);
                    }
                    else {
                        // Trampolim '\'
                        alturaNoPonto = boundsOutra.size.y * pctTrampolim;
                    }
                    return boundsOutra.position.y + alturaNoPonto;
                };

                // Calcula a altura "correta" do chão da rampa
                float yTopoTrampolim = calcularAlturaTrampolim(centroProprio.x);
                
                // Pega a posição dos "pés" do jogador
                float peY = boundsPropria.position.y + boundsPropria.size.y;

                // --- MUDANÇA PRINCIPAL AQUI ---
                // Condição:
                // 1. O jogador está caindo ou parado (velocidade.y >= 0)
                // 2. Os pés do jogador estão *perto ou abaixo* da linha da rampa
                
                // (Aumentei a tolerância para "pegar" o jogador mais facilmente)
                float stepUpTolerance = 5.f; 
                
                if (peY >= (yTopoTrampolim - stepUpTolerance) && velocidade.y >= 0)
                {
                    // "Eleve o boneco para a linha da diagonal"
                    // Calcula a nova posição Y exata do *topo* do sprite
                    float newPlayerY = yTopoTrampolim - boundsPropria.size.y;
                    
                    // FORÇA a posição do jogador para a altura correta
                    // (Mantém a posição X que ele já tinha)
                    sprite->setPosition({ posAtual.x, newPlayerY });

                    // O resto da lógica
                    velocidade.y = 0.f;
                    podePular = true;
                    estaNaTrampolim = true;
                    rampaSobeEsquerda = pTrampolim->getSobeDaEsquerda();
                }
                else
                {
                    // O jogador está pulando por *baixo* da rampa.
                    // Trata a colisão apenas como horizontal.
                    if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                    else { sprite->move({ -overlapX, 0.f }); }
                    velocidade.x = 0.f;
                }
            }
            else{

                if (overlapY < overlapX)
                {
                    // Colisão Vertical
                    if (distCentros.y > 0)
                    {
                        // Colidiu com o teto
                        sprite->move({ 0.f, overlapY });
                        velocidade.y = 0.f;
                    }
                    else
                    {
                        // Colidiu com o chão
                        if (velocidade.y >= 0)
                        {
                            sprite->move({ 0.f, -overlapY });
                            velocidade.y = 0.f;
                            podePular = true;
                            estaNaTrampolim = false; // Garante que não está mais na rampa
                        }
                    }
                }
                else
                {
                    // Colisão Horizontal
                    float peJogador = boundsPropria.position.y + boundsPropria.size.y;
                    float topoPlataforma = boundsOutra.position.y;

                    // Ignora colisão horizontal se for só uma "quina"
                    if (peJogador <= topoPlataforma + 1.f) { /* Ignora */ }
                    else
                    {
                        if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                        else { sprite->move({ -overlapX, 0.f }); }
                        velocidade.x = 0.f;
                    }
                }
            }
        }
    }
}