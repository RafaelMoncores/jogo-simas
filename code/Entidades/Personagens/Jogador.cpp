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
        Jogador::Jogador() :
            Personagem(),
            VELOCIDADE_MAXIMA_LATERAL(350.0f),
            ACELERACAO_LATERAL(900.0f),
            FRICCAO_LATERAL(1200.0f),
            FORCA_PULO(420.0f),
            MULTIPLICADOR_PULO_CURTO(3.0f),
            podePular(false)
        {
            if (!textura.loadFromFile("player.png"))
            {
                std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
                exit(1);
            }

            sprite.emplace(textura);
            sprite->setPosition({400.0f, 300.0f}); 
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
                
                /*
                float chaoFalso = 800.0f;
                sf::Vector2f pos = sprite->getPosition();

                if (pos.y > chaoFalso)
                {
                    sprite->setPosition({pos.x, chaoFalso});
                    velocidade.y = 0.0f;
                    podePular = true; 
                }*/
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

                

                // 1. "Apertar" horizontalmente (corrige o "coyote time")
                // Vamos supor que 8 pixels de cada lado são vazios
                float ajusteLargura = 48.0f; 
                bounds.position.x += ajusteLargura / 2.f;
                bounds.size.x -= ajusteLargura;

                // 2. "Abaixar" a caixa (corrige o "flutuante")
                // Vamos supor que os 10 pixels de baixo do sprite são transparentes
                float ajusteAltura = 10.0f; 
                bounds.size.y -= ajusteAltura;

                return bounds;
            }
            return {};
        }

        // --- ADICIONAR ESTES MÉTODOS ---

        void Jogador::setPodePular(bool pode)
        {
            podePular = pode;
        }

        void Jogador::resolverColisao(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            if (!sprite) return;
            sf::FloatRect boundsPropria = getBoundingBox();

            // -----------------------------------------------------------------
            // TENTATIVA DE COLISÃO COM RAMPA
            // -----------------------------------------------------------------
            if (auto* pRampa = dynamic_cast<Obstaculos::Rampa*>(pOutra))
            {
                // Só colidimos com rampa se estivermos caindo ou andando sobre ela
                if (velocidade.y < -0.1f) return; // Se estiver pulando para cima, ignora

                // --- INÍCIO DA CORREÇÃO ---
                //
                // Posição dos "pés" do jogador (canto esquerdo e direito)
                float peY = boundsPropria.position.y + boundsPropria.size.y;
                float peX_esq = boundsPropria.position.x;
                float peX_dir = boundsPropria.position.x + boundsPropria.size.x;

                // Função local para calcular a altura da rampa em um ponto X
                auto calcularAlturaRampa = [&](float pontoX) -> float 
                {
                    float xRelativo = pontoX - boundsOutra.position.x;
                    float pctRampa = std::clamp(xRelativo / boundsOutra.size.x, 0.f, 1.f); // Trava entre 0 e 1

                    float alturaNoPonto;
                    if (pRampa->getSobeDaEsquerda()) // Rampa tipo '/'
                    {
                        alturaNoPonto = boundsOutra.size.y * (1.f - pctRampa);
                    }
                    else // Rampa tipo '\'
                    {
                        alturaNoPonto = boundsOutra.size.y * pctRampa;
                    }
                    return boundsOutra.position.y + alturaNoPonto;
                };

                // Calcula a altura da rampa nos dois cantos do pé
                float yTopoRampa_Esq = calcularAlturaRampa(peX_esq);
                float yTopoRampa_Dir = calcularAlturaRampa(peX_dir);

                // O topo real é o ponto MAIS ALTO (menor Y) dos dois
                float yTopoRampa = std::min(yTopoRampa_Esq, yTopoRampa_Dir);

                // --- FIM DA CORREÇÃO ---

                // Se o pé do jogador (peY) está abaixo do topo da rampa
                if (peY > yTopoRampa)
                {
                    // Move o jogador para cima da rampa
                    // Usamos yTopoRampa - 1.f para "afundar" 1 pixel e garantir atrito
                    sprite->setPosition({ sprite->getPosition().x, yTopoRampa - boundsPropria.size.y - 1.f });
                    velocidade.y = 0.f;
                    podePular = true;
                }
            }
            // -----------------------------------------------------------------
            // TENTATIVA DE COLISÃO COM PLATAFORMA (Lógica antiga)
            // -----------------------------------------------------------------
            else if (dynamic_cast<Obstaculos::Plataforma*>(pOutra))
            {
                sf::Vector2f posAtual = sprite->getPosition();

                // (Aqui entra toda aquela lógica de AABB que já fizemos)
                sf::Vector2f centroProprio(boundsPropria.position.x + boundsPropria.size.x / 2.f, boundsPropria.position.y + boundsPropria.size.y / 2.f);
                sf::Vector2f centroOutra(boundsOutra.position.x + boundsOutra.size.x / 2.f, boundsOutra.position.y + boundsOutra.size.y / 2.f);
                sf::Vector2f distCentros(centroProprio.x - centroOutra.x, centroProprio.y - centroOutra.y);
                float somaMeiasLarguras = boundsPropria.size.x / 2.f + boundsOutra.size.x / 2.f;
                float somaMeiasAlturas = boundsPropria.size.y / 2.f + boundsOutra.size.y / 2.f;
                float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
                float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

                if (overlapX > 0 && overlapY > 0) 
                {
                    if (overlapY < overlapX)
                    {
                        if (distCentros.y > 0) // Colisão por baixo (cabeça)
                        {
                            sprite->setPosition({ posAtual.x, posAtual.y + overlapY });
                            velocidade.y = 0.f; 
                        }
                        else // Colisão por cima (pés)
                        {
                            sprite->setPosition({ posAtual.x, posAtual.y - overlapY });
                            velocidade.y = 0.f;
                            podePular = true;
                        }
                    }
                    else // Colisão lateral
                    {
                        // --- INÍCIO DA CORREÇÃO ---
                        //
                        // Verifique se o PÉ do jogador está ACIMA do TOPO da plataforma
                        float peJogador = boundsPropria.position.y + boundsPropria.size.y;
                        float topoPlataforma = boundsOutra.position.y;

                        // Se o pé do jogador estiver mais alto (menor Y) ou igual ao topo da plataforma,
                        // é uma "quina" de rampa. NÃO colida lateralmente.
                        if (peJogador <= topoPlataforma + 1.f) // +1.f para uma pequena margem
                        {
                            // Não faz nada. Permite que o jogador "suba" a quina.
                        }
                        else // Caso contrário, é uma colisão de parede real.
                        {
                            if (distCentros.x > 0) // Pela direita
                            {
                                sprite->setPosition({ posAtual.x + overlapX, posAtual.y });
                            }
                            else // Pela esquerda
                            {
                                sprite->setPosition({ posAtual.x - overlapX, posAtual.y });
                            }
                            velocidade.x = 0.f;
                        }
                        // --- FIM DA CORREÇÃO ---
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