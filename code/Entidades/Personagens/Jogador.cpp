#include "Jogador.hpp"
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

                float chaoFalso = 800.0f;
                sf::Vector2f pos = sprite->getPosition();

                if (pos.y > chaoFalso)
                {
                    sprite->setPosition({pos.x, chaoFalso});
                    velocidade.y = 0.0f;
                    podePular = true; 
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
    }
}