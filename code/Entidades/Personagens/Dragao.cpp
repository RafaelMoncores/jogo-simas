#include "Dragao.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include <iostream>
#include <cmath>

namespace Entidades
{
    namespace Personagens
    {
        Dragao::Dragao(sf::Vector2f pos) :
            Inimigo(20, pos),
            forca(2)
        {
            if (!textura.loadFromFile("tileSets/dragao.png"))
            {
                std::cerr << "ERRO FATAL: Nao foi possivel carregar 'dragao.png'" << std::endl;
                exit(1);
            }
            
            sprite.emplace(textura); 
            sprite->setPosition(posInicial);
            sprite->setScale({3,3});
            
            nivel_maldade = 3;
        }

        Dragao::~Dragao()
        {
        }

        void Dragao::executar(float delta)
        {
            if (num_vidas <= 0)
            {
                if (sprite)
                {
                    sprite.reset();
                }
                return;
            }

            aplicarFisica(delta);
        }

        void Dragao::aplicarFisica(float delta)
        {
            if (sprite) 
            {
                velocidade.y += G_ACCEL.y * delta;
                sprite->move(velocidade * delta);

                float deathPlaneY = 1500.f;
                if (sprite->getPosition().y > deathPlaneY)
                {
                    perderVida(num_vidas);
                }
            }
        }
        
        void Dragao::danificar(Personagem* pOutro)
        {
            if (num_vidas <= 0) return;
            pOutro->perderVida(forca); 
        }

        void Dragao::colidir(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            if (num_vidas <= 0) return;
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

        void Dragao::salvarDataBuffer()
        {
            Inimigo::salvarDataBuffer();
        }

        void Dragao::salvar()
        {
            Inimigo::salvar();
        }
    }
}