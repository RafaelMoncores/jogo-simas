#include "Gosma.hpp"
#include <math.h>
#include <iostream>
#include <stdlib.h>

namespace Entidades
{
    namespace Personagens
    {
        Gosma::Gosma(sf::Vector2f pos, Obstaculos::Plataforma* pPlat, Entidades::Personagens::Jogador* pJ) :
            Inimigo(1, pos),
            pPlataforma(pPlat),
            tempoEspera(2.0f),
            tempoTotal(0.0f),
            direcaoPulo(1),
            FORCA_PULO_INIMIGO(350.0f),
            VELOCIDADE_LATERAL_INIMIGO(100.0f),
            pJogador(pJ),
            raio_deteccao(250.0f)
        {
            if (!textura.loadFromFile("gosma.png"))
            {
                std::cerr << "ERRO FATAL: Nao foi possivel carregar 'Gosma.png'" << std::endl;
                exit(1);
            }
            
            sprite.emplace(textura); 
            sprite->setPosition(posInicial);

            nivel_maldade = 1;
        }

        Gosma::~Gosma()
        {
        }

        void Gosma::processarAI(float delta)
        {
            if (num_vidas <= 0) return;
            if (!pPlataforma || !sprite || !pJogador) return;

            if (podePular)
            {
                tempoTotal += delta;
                velocidade.x = 0.f; 

                if (tempoTotal >= tempoEspera)
                {
                    sf::Vector2f posGosma = sprite->getPosition();
                    sf::Vector2f posPlayer = pJogador->getBoundingBox().position;
                    
                    float dist = std::sqrt(
                        std::pow(posGosma.x - posPlayer.x, 2) + 
                        std::pow(posGosma.y - posPlayer.y, 2)
                    );

                    if (dist < raio_deteccao)
                    {
                        if (posPlayer.x < posGosma.x)
                            direcaoPulo = -1;
                        else
                            direcaoPulo = 1; 
                    }
                    else
                    {
                        sf::FloatRect boundsPlat = pPlataforma->getBoundingBox();
                        float xAtual = posGosma.x;
                        float xCentroPlataforma = boundsPlat.position.x + (boundsPlat.size.x / 2.f); 
                        
                        if (xAtual < xCentroPlataforma) direcaoPulo = 1;
                        else direcaoPulo = -1;
                    }

                    velocidade.y = -FORCA_PULO_INIMIGO;
                    velocidade.x = VELOCIDADE_LATERAL_INIMIGO * direcaoPulo;
                    tempoTotal = 0.0f;
                    podePular = false;
                }
            }
        }
        
        void Gosma::aplicarFisica(float delta)
        {
            if (sprite) 
            {
                velocidade.y += G_ACCEL.y * delta;
                velocidade.x = std::clamp(velocidade.x, -VELOCIDADE_MAXIMA_LATERAL, VELOCIDADE_MAXIMA_LATERAL);

                sprite->move(velocidade * delta);

                float deathPlaneY = 1500.f;
                if (sprite->getPosition().y > deathPlaneY)
                {
                    perderVida();
                }
            }
        }

        void Gosma::executar(float delta)
        {
            if (num_vidas <= 0)
            {
                if (sprite)
                {
                    sprite.reset();
                }
                return;
            }
            processarAI(delta);
            aplicarFisica(delta);
        }

        void Gosma::danificar(Personagem* pOutro)
        {
            if (num_vidas <= 0) return;
            pOutro->perderVida();
        }

        void Gosma::colidir(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            if (num_vidas <= 0) return;
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

            if (dynamic_cast<Obstaculos::Rampa*>(pOutra))
            {
                if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                else { sprite->move({ -overlapX, 0.f }); }
                velocidade.x = 0.f;
                return;
            }

            if (overlapY < overlapX)
            {
                if (distCentros.y > 0)
                { 
                    sprite->move({ 0.f, overlapY }); 
                }
                else
                { 
                    sprite->move({ 0.f, -overlapY }); 
                    if (velocidade.y >= 0)
                    {
                        podePular = true;
                        velocidade.y = 0.f;
                    }
                }
            }
            else
            {
                if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                else { sprite->move({ -overlapX, 0.f }); }
                velocidade.x = 0.f;
            }
        }

        void Gosma::salvarDataBuffer()
        {
            Inimigo::salvarDataBuffer();
        }

        void Gosma::salvar()
        {
            Inimigo::salvar();
        }
    }
}