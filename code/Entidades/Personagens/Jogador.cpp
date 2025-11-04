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
            Personagem(),
            VELOCIDADE_MAXIMA_LATERAL(350.0f),
            ACELERACAO_LATERAL(900.0f),
            FRICCAO_LATERAL(1200.0f),
            FORCA_PULO(420.0f),
            MULTIPLICADOR_PULO_CURTO(3.0f),
            podePular(false),
            posInicial(pos)
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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
            {
                velocidade.x -= ACELERACAO_LATERAL * delta;
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
            {
                velocidade.x += ACELERACAO_LATERAL * delta;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && podePular)
            {
                velocidade.y = -FORCA_PULO; 
                podePular = false;
            }
        }

        void Jogador::aplicarFisica(float delta)
        {
            if (sprite) 
            {
                if (velocidade.y < 0.0f && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    velocidade.y += G_ACCEL.y * MULTIPLICADOR_PULO_CURTO * delta;
                }
                else
                {
                    velocidade.y += G_ACCEL.y * delta;
                }

                bool noInputHorizontal = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && 
                                    !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

                if (podePular && noInputHorizontal)
                {
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
                
                velocidade.x = std::clamp(velocidade.x, -VELOCIDADE_MAXIMA_LATERAL, VELOCIDADE_MAXIMA_LATERAL);

                sprite->move(velocidade * delta);

                float deathPlaneY = 1500.f;

                if (sprite->getPosition().y > deathPlaneY)
                {
                    perderVida();
                    std::cout << "Jogador caiu! Vidas restantes: " << getVidas() << std::endl;

                    setPosition(posInicial);
                    velocidade = {0.f, 0.f};
                }
            }
        }

        void Jogador::executar(float delta)
        {
            processarInputs(delta);
            aplicarFisica(delta);
        }

        void Jogador::desenhar()
        {
            if (pGG && sprite)
            {
                pGG->desenhar(*sprite);
            }
        }

        sf::FloatRect Jogador::getBoundingBox() const
        {
            if (sprite)
            {
                sf::FloatRect bounds = sprite->getGlobalBounds();
                float ajusteLargura = 48.0f; 

                bounds.position.x += ajusteLargura / 2.f;
                bounds.size.x -= ajusteLargura;

                float ajusteAltura = 10.0f; 
                bounds.size.y -= ajusteAltura;

                return bounds;
            }
            return {};
        }

        void Jogador::setPodePular(bool pode)
        {
            podePular = pode;
        }

        void Jogador::resolverColisao(Entidade* pOutra, sf::FloatRect boundsOutra)
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

            if (auto* pRampa = dynamic_cast<Obstaculos::Rampa*>(pOutra))
            {
                float peY = boundsPropria.position.y + boundsPropria.size.y;
                float peX_esq = boundsPropria.position.x;
                float peX_dir = boundsPropria.position.x + boundsPropria.size.x;

                auto calcularAlturaRampa = [&](float pontoX) -> float 
                {
                    float xRelativo = pontoX - boundsOutra.position.x;
                    float pctRampa = std::clamp(xRelativo / boundsOutra.size.x, 0.f, 1.f);
                    float alturaNoPonto;
                    if (pRampa->getSobeDaEsquerda())
                    {
                        alturaNoPonto = boundsOutra.size.y * (1.f - pctRampa);
                    }
                    else
                    {
                        alturaNoPonto = boundsOutra.size.y * pctRampa;
                    }
                    return boundsOutra.position.y + alturaNoPonto;
                };

                float yTopoRampa = std::min(calcularAlturaRampa(peX_esq), calcularAlturaRampa(peX_dir));
                float overlapRampa = peY - yTopoRampa;

                if (overlapRampa > 0 && velocidade.y >= 0)
                {
                    sprite->move({ 0.f, -overlapRampa }); // Usa 'move'
                    velocidade.y = 0.f;
                    podePular = true;
                }
                else
                {
                    if (overlapY < overlapX) // Colisão de Cabeça
                    {
                        if (distCentros.y > 0)
                        {
                            // --- MUDANÇA (Jitter Fix) ---
                            sprite->move({ 0.f, overlapY }); // Usa 'move'
                            velocidade.y = 0.f; 
                        }
                    }
                    else // Colisão Lateral
                    {
                        // --- MUDANÇA (Jitter Fix) ---
                        if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                        else { sprite->move({ -overlapX, 0.f }); }
                        velocidade.x = 0.f;
                    }
                }
            }
            else
            {
                if (overlapY < overlapX)
                {
                    if (distCentros.y > 0)
                    {
                        sprite->move({ 0.f, overlapY });
                        velocidade.y = 0.f; 
                    }
                    else
                    {
                        if (velocidade.y >= 0)
                        {
                            sprite->move({ 0.f, -overlapY });
                            velocidade.y = 0.f;
                            podePular = true;
                        }
                    }
                }
                else
                {
                    float peJogador = boundsPropria.position.y + boundsPropria.size.y;
                    float topoPlataforma = boundsOutra.position.y;

                    if (peJogador <= topoPlataforma + 1.f) { /* Ignora (quina) */ }
                    else
                    {
                        if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                        else { sprite->move({ -overlapX, 0.f }); }
                        velocidade.x = 0.f;
                    }
                }
            }
        }

        void Jogador::setPosition(sf::Vector2f pos)
        {
            if (sprite)
            {
                sprite->setPosition(pos);
            }   
        }
    }
}