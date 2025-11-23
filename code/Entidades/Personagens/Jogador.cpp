#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/PlataformaFinal.hpp"
#include "../Obstaculos/Trampolim.hpp"
#include <iostream>
#include <stdlib.h>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace Entidades
{
    namespace Personagens
    {

        Jogador::Jogador(sf::Vector2f pos, int id) : // Recebe 'id'
            Personagem(3, pos),
            FORCA_PULO(420.0f),
            MULTIPLICADOR_PULO_CURTO(3.0f),
            direcao(1),
            estaAtacando(false),
            tempoAtaque(0.0f),
            COOLDOWN_ATAQUE(0.3f),
            completouFase(false),
            pontos(1000.0f),
            idJogador(id)
        {
            std::string texturaPath;
            if (idJogador == 1)
            {
                texturaPath = "tileSets/player.png";
                teclaCima = sf::Keyboard::Key::W;
                teclaEsq = sf::Keyboard::Key::A;
                teclaDir = sf::Keyboard::Key::D;
                teclaAtaque = sf::Keyboard::Key::F;
            }
            else
            {
                texturaPath = "tileSets/player2.png";
                teclaCima = sf::Keyboard::Key::Up;
                teclaEsq = sf::Keyboard::Key::Left;
                teclaDir = sf::Keyboard::Key::Right;
                teclaAtaque = sf::Keyboard::Key::Space;
            }

            if (!textura.loadFromFile(texturaPath)) //lança o erro para a main
            {
                throw std::runtime_error("Erro: Nao foi possivel carregar a textura do Jogador: " + texturaPath);
            }
            
            sprite.emplace(textura); 
            sprite->setPosition(posInicial);
        }

        Jogador::~Jogador()
        {
        }

        void Jogador::processarInputs(float delta)
        {
            if (sf::Keyboard::isKeyPressed(teclaCima) && podePular)
            {
                velocidade.y = -FORCA_PULO;
                podePular = false;
            }
            if (sf::Keyboard::isKeyPressed(teclaAtaque) && !estaAtacando)
            {
                estaAtacando = true;
                tempoAtaque = COOLDOWN_ATAQUE;
            }
        }

        void Jogador::aplicarFisica(float delta)
        {
            const float FATOR_LENTIDAO_RAMPA = 0.30f;
            float modificadorAceleracao = 0.7f; 

            bool inputEsquerda = sf::Keyboard::isKeyPressed(teclaEsq);
            bool inputDireita = sf::Keyboard::isKeyPressed(teclaDir);
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

            if (velocidade.y < 0.0f && !sf::Keyboard::isKeyPressed(teclaCima))
            {
                velocidade.y += G_ACCEL.y * MULTIPLICADOR_PULO_CURTO * delta;
            }

            velocidade.y += G_ACCEL.y * delta;

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
            if (completouFase) return;

            if (num_vidas > 0)
            {
                processarInputs(delta);

                if (estaAtacando && tempoAtaque > 0.f)
                {
                    tempoAtaque -= delta;
                    if (tempoAtaque <= 0.f)
                    {
                        estaAtacando = false;
                    }
                }

                pontos -= 20.0f * delta;
                if (pontos < 0.f) pontos = 0.f;
            }
            
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
                if (distCentros.y > 0)
                {
                    sprite->move({ 0.f, overlapY });
                    velocidade.y = 0.f;
                }
                else
                {
                    if (velocidade.y >= 0)
                    {
                        if (auto* pFinal = dynamic_cast<Obstaculos::PlataformaFinal*>(pOutra))
                        {
                            float xJogador = centroProprio.x;
                            
                            float meioInicio = boundsOutra.position.x + (boundsOutra.size.x * 0.25f);
                            float meioFim = boundsOutra.position.x + (boundsOutra.size.x * 0.75f);

                            if (xJogador > meioInicio && xJogador < meioFim)
                            {
                                completouFase = true;
                                velocidade = {0.f, 0.f};
                                return;
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
    
        int Jogador::getPontos() const 
        {
            return static_cast<int>(pontos);
        }

        void Jogador::salvarDataBuffer()
        {
            Personagem::salvarDataBuffer();
            if (!buffer) return;
            (*buffer) << "Jogador" << std::endl;
            (*buffer) << "idJogador " << idJogador << std::endl;
            (*buffer) << "pontos " << pontos << std::endl;
            (*buffer) << "completouFase " << (completouFase ? 1 : 0) << std::endl;
            (*buffer) << "direcao " << direcao << std::endl;
        }

        sf::FloatRect Jogador::getHitboxAtaque() const
        {
            if (!estaAtacando || !sprite) 
                return {};

            sf::FloatRect boundsJogador = sprite->getGlobalBounds();
            
            sf::Vector2f attackSize = { 50.f, boundsJogador.size.y * 0.8f }; 
            sf::Vector2f attackPos;

            if (direcao == 1)
            {
                attackPos.x = boundsJogador.position.x + boundsJogador.size.x;
            }
            else
            {
                attackPos.x = boundsJogador.position.x - attackSize.x;
            }
            
            attackPos.y = boundsJogador.position.y + (boundsJogador.size.y - attackSize.y) / 2.f;

            return { attackPos, attackSize };
        }

        void Jogador::addPontos(int p) 
        {
            pontos += p;
        }

        void Jogador::setPontos(float p) 
        {
            pontos = p;
            if (pontos < 0.f) pontos = 0.f;
        }

        void Jogador::pular()
        {
            // Usa a mesma lógica que estava no processarInputs
            if (podePular)
            {
                velocidade.y = -std::sqrt(2.0f * 981.0f * 200.0f);
                podePular = false;
            }
        }
    
    }
}