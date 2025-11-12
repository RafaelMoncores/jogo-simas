#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/Rampa.hpp"
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
            estaNaRampa(false),
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

            if (estaNaRampa)
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

            if (estaNaRampa) 
            {
                // ESTAMOS NA RAMPA
                const float FORCA_SLIDE_RAMPA = 10.0f; // O seu valor de slide

                // SÓ APLIQUE O SLIDE SE O JOGADOR ESTIVER PARADO
                if (noInputHorizontal)
                {
                    // Rampa '/' (sobe da esquerda) -> desliza para a ESQUERDA (negativo)
                    if (rampaSobeEsquerda) 
                    {
                        velocidade.x -= FORCA_SLIDE_RAMPA * delta; // <--- CORRIGIDO
                    }
                    // Rampa '\' (sobe da direita) -> desliza para a DIREITA (positivo)
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
                // Salva posição antes de mover — usado depois pelo sistema de colisão
                previousPosition = sprite->getPosition();

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

            sf::Vector2f centroProprio(boundsPropria.position.x + boundsPropria.size.x / 2.f, boundsPropria.position.y + boundsPropria.size.y / 2.f);
            sf::Vector2f centroOutra(boundsOutra.position.x + boundsOutra.size.x / 2.f, boundsOutra.position.y + boundsOutra.size.y / 2.f);
            sf::Vector2f distCentros(centroProprio.x - centroOutra.x, centroProprio.y - centroOutra.y);

            float somaMeiasLarguras = boundsPropria.size.x / 2.f + boundsOutra.size.x / 2.f;
            float somaMeiasAlturas = boundsPropria.size.y / 2.f + boundsOutra.size.y / 2.f;
            float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
            float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

            if (overlapX <= 0 || overlapY <= 0) return;

            if (auto* pRampa = dynamic_cast<Obstaculos::Rampa*>(pOutra))
            {
                // --- INÍCIO DA LÓGICA DA RAMPA (melhorada) ---
                auto calcularAlturaRampa = [&](float pontoX) -> float {
                    // força o pontoX para dentro da largura da rampa — evita pct fora de [0,1]
                    float xRelativo = pontoX - boundsOutra.position.x;
                    xRelativo = std::clamp(xRelativo, 0.f, boundsOutra.size.x);
                    float pctRampa = xRelativo / boundsOutra.size.x;
                    float alturaNoPonto;
                    if (pRampa->getSobeDaEsquerda()){
                        alturaNoPonto = boundsOutra.size.y * (1.f - pctRampa);
                    } else {
                        alturaNoPonto = boundsOutra.size.y * pctRampa;
                    }
                    return boundsOutra.position.y + alturaNoPonto;
                };

                float peY = boundsPropria.position.y + boundsPropria.size.y;

                // amostrar nos dois "pés" do jogador (evita teleporte quando o jogador entra no meio)
                float leftFootX  = boundsPropria.position.x + 1.f;
                float rightFootX = boundsPropria.position.x + boundsPropria.size.x - 1.f;

                float yLeft  = calcularAlturaRampa(leftFootX);
                float yRight = calcularAlturaRampa(rightFootX);

                float overlapLeft  = peY - yLeft;
                float overlapRight = peY - yRight;

                // usaremos a penetração máxima de um dos pés para decidir a correção vertical
                float overlapRampa = std::max(overlapLeft, overlapRight);
                float stepUpTolerance = 2.f;

                // Se um dos pés estiver perto/penetrando a superfície e estivermos caindo
                if (overlapRampa > -stepUpTolerance &&
                    (velocidade.y >= 0.f || (velocidade.y < 0.f && overlapRampa > 0.f)))
                {
                    if (overlapRampa > 0.f)
                    {
                        // corrige o jogador para ficar sobre a rampa
                        sprite->move({ 0.f, -overlapRampa });
                    }
                    velocidade.y = 0.f;
                    podePular = true;

                    estaNaRampa = true;
                    rampaSobeEsquerda = pRampa->getSobeDaEsquerda();
                }
                else
                {
                    // O jogador bateu na lateral/por baixo da rampa.
                    // Evitamos "grudar" na quina: se no frame anterior o pé estava acima da rampa,
                    // permitimos um pequeno step-up em vez de empurrar horizontalmente.
                    float previousBottom = previousPosition.y + boundsPropria.size.y;

                    // yMin é a parte mais alta da rampa sob o jogador (usada como referência)
                    float yMinUnderPlayer = std::min(yLeft, yRight);

                    if (previousBottom <= yMinUnderPlayer + stepUpTolerance && overlapRampa > -stepUpTolerance)
                    {
                        // permitimos subir um pouco — trata como pé encontrando a rampa
                        if (overlapRampa > 0.f)
                        {
                            sprite->move({ 0.f, -overlapRampa });
                            velocidade.y = 0.f;
                            podePular = true;
                            estaNaRampa = true;
                            rampaSobeEsquerda = pRampa->getSobeDaEsquerda();
                        }
                        else
                        {
                            // caso extremo: nada a fazer verticalmente -> empurrar horizontal leve
                            if (distCentros.x > 0) sprite->move({ overlapX * 0.5f, 0.f });
                            else sprite->move({ -overlapX * 0.5f, 0.f });
                            velocidade.x = 0.f;
                        }
                    }
                    else
                    {
                        // colisão lateral "normal" -> empurra horizontalmente
                        if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                        else { sprite->move({ -overlapX, 0.f }); }
                        velocidade.x = 0.f;
                    }
                }
                // --- FIM DA LÓGICA DA RAMPA ---
            }
            else
            {
                // --- INÍCIO DA LÓGICA AABB (Plataforma/Parede) ---
                // (Bug 2)
                
                float stepUpTolerance = 2.f; // A mesma tolerância é necessária aqui

                if (overlapY < overlapX)
                {
                    // Colisão Vertical
                    if (distCentros.y < 0) // Colidiu com o CHÃO
                    {
                        // Só aplique a colisão com o chão se:
                        // 1. Estivermos a cair (vel.y >= 0)
                        // 2. Estivermos perto da superfície (overlapY > -tol)
                        if (velocidade.y >= 0 && overlapY > -stepUpTolerance)
                        {
                            if (overlapY > 0) // Só mova se estiver afundado
                            {
                                sprite->move({ 0.f, -overlapY });
                            }
                            velocidade.y = 0.f;
                            podePular = true;
                            estaNaRampa = false; 
                        }
                    }
                    else // Colidiu com o TETO
                    {
                        sprite->move({ 0.f, overlapY });
                        velocidade.y = 0.f;
                    }
                }
                else
                {
                    // Colisão Horizontal (Parede)
                    if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                    else { sprite->move({ -overlapX, 0.f }); }
                    velocidade.x = 0.f;
                }
                // --- FIM DA LÓGICA AABB ---
            }
        }
    }
}