#include "Personagem.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <SFML/Window/Keyboard.hpp>

namespace Entidades
{
    namespace Personagens
    {
        Personagem::Personagem(int vidas, sf::Vector2f pos) :
            Entidade(),
            num_vidas(vidas),
            sprite(),
            posInicial(pos),
            VELOCIDADE_MAXIMA_LATERAL(350.0f),
            ACELERACAO_LATERAL(900.0f),
            FRICCAO_LATERAL(1200.0f),
            podePular(false)
        {
        }

        Personagem::~Personagem()
        {
        }

        void Personagem::perderVida(int dano)
        {
            num_vidas -= dano;
            if (num_vidas < 0) num_vidas = 0;
            
            if(num_vidas > 0)
            {
                std::cout << "Um personagem perdeu " << dano << " vida(s)! Vidas restantes: " << getVidas() << std::endl;
                setPosition(posInicial);
                velocidade = {0.f, 0.f};
            }
            else
            {
                velocidade = {0.f, 0.f};
            }
        }

        int Personagem::getVidas() const
        {
            return num_vidas;
        }

        sf::Vector2f Personagem::getVelocidade() const
        {
            return velocidade;
        }

        void Personagem::setPosition(sf::Vector2f pos)
        {
            if (sprite)
            {
                sprite->setPosition(pos);
            }   
        }

        void Personagem::desenhar()
        {
            if (num_vidas <= 0) return;
            if (pGG && sprite)
            {
                pGG->desenhar(*sprite);
            }
        }

        sf::FloatRect Personagem::getBoundingBox() const
        {
            if (num_vidas <= 0) return{};
            if (sprite)
            {
                return sprite->getGlobalBounds();
            }
            return {};
        }

        void Personagem::setPodePular(bool pode)
        {
            podePular = pode;
        }

        void Personagem::aplicarFisica(float delta)
        {
            if (sprite)
            {
                velocidade.y += G_ACCEL.y * delta;

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
                }
            }
        }

        void Personagem::fazerBounce(float forcaBounce)
        {
            velocidade.y = -forcaBounce; 
        }
    }
}