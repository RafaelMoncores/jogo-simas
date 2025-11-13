#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/PlataformaFinal.hpp"
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
            direcao(1),
            estaAtacando(false),
            tempoAtaque(0.0f),
            COOLDOWN_ATAQUE(0.3f),
            completouFase(false)
        {
            if (!textura.loadFromFile("tileSets/player.png"))
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

            if (podePular && noInputHorizontal)
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

        bool Jogador::getCompletouFase() const
        {
            return completouFase;
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

            if (overlapY < overlapX)
            {
                // Colisão Vertical
                if (distCentros.y > 0) // Teto
                {
                    sprite->move({ 0.f, overlapY });
                    velocidade.y = 0.f;
                }
                else // Chão
                {
                    if (velocidade.y >= 0)
                    {
                        // --- LÓGICA DE CONCLUSÃO DE FASE ---
                        // Tenta converter o obstáculo para PlataformaFinal
                        if (auto* pFinal = dynamic_cast<Obstaculos::PlataformaFinal*>(pOutra))
                        {
                            // Pega o centro X do jogador
                            float xJogador = centroProprio.x;
                            
                            // Calcula o "meio" da plataforma (os 50% centrais)
                            float meioInicio = boundsOutra.position.x + (boundsOutra.size.x * 0.25f);
                            float meioFim = boundsOutra.position.x + (boundsOutra.size.x * 0.75f);

                            // Se o jogador está no meio...
                            if (xJogador > meioInicio && xJogador < meioFim)
                            {
                                completouFase = true; // Sinaliza que a fase terminou!
                                velocidade = {0.f, 0.f}; // Para o jogador
                                return; // Interrompe a física normal
                            }
                        }
                        sprite->move({ 0.f, -overlapY });
                        velocidade.y = 0.f;
                        podePular = true;
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