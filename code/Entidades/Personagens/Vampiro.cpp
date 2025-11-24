#include "Vampiro.hpp"
#include <iostream>
#include <cmath>
#include "../../Gerenciadores/GerenciadorRecursos.hpp"
namespace Entidades
{
    namespace Personagens
    {
        Vampiro::Vampiro(sf::Vector2f pos, float tam, Jogador* pJ1, Jogador* pJ2) :
            Inimigo(2, pos, pJ1, pJ2),
            tamanho(tam),
            tempoTotal(0.0f),
            direcao(1),
            VELOCIDADE_PATRULHA(150.0f),
            AMPLITUDE_SENOIDE(120.0f),
            FREQUENCIA_SENOIDE(2.5f),
            LIMITE_ESQUERDA(50.0f),
            LIMITE_DIREITA(1870.0f)
        {
            
            sprite.emplace(Gerenciadores::GerenciadorRecursos::getInstance()->getTextura("tileSets/vampiro.png")); 
            sprite->setPosition(posInicial);
            sprite->setScale({tamanho, tamanho});
            
            nivel_maldade = 2;
        }

        Vampiro::~Vampiro()
        {
        }

        void Vampiro::executar(float delta)
        {
            if (num_vidas <= 0)
            {
                if (sprite)
                {
                    sprite.reset();
                }
                return;
            }

            tempoTotal += delta;
            
            processarAI(delta);
            velocidade.y -= G_ACCEL.y * delta;
            aplicarFisica(delta);
        }
        
        void Vampiro::processarAI(float delta)
        {
            if (num_vidas <= 0) return;
            if (!sprite) return;

            float posX = sprite->getPosition().x;

            if (posX > LIMITE_DIREITA)
            {
                direcao = -1;
            }
            else if (posX < LIMITE_ESQUERDA)
            {
                direcao = 1;
            }
            
            velocidade.x = VELOCIDADE_PATRULHA * direcao;
            velocidade.y = 0; 
        }

        void Vampiro::aplicarFisica(float delta)
        {
            if (!sprite || num_vidas <= 0) return;
            velocidade.y += G_ACCEL.y * delta;

            float newX = sprite->getPosition().x + (velocidade.x * delta);
            float newY = posInicial.y + (AMPLITUDE_SENOIDE * std::sin(FREQUENCIA_SENOIDE * tempoTotal));

            sprite->setPosition({newX, newY});
        }

        void Vampiro::danificar(Personagem* pOutro)
        {
            if (num_vidas <= 0) return;

            if (pOutro)
            {
                pOutro->perderVida(1); 
            }
            else 
            {
                perderVida(1); 
                
                Jogador* pAlvo = getJogadorMaisProximo();
                if (pAlvo)
                {
                    pAlvo->addPontos(300); 
                }
            }
        }

        void Vampiro::colidir(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            if (num_vidas <= 0) return;
            return;
        }

        sf::FloatRect Vampiro::getBoundingBox() const
        {
            if (num_vidas <= 0) return {};

            if (sprite)
            {
                return sprite->getGlobalBounds();
            }
            return {};
        }

        void Vampiro::salvarDataBuffer()
        {
            Inimigo::salvarDataBuffer();
            if (!buffer) return;
            (*buffer) << "Vampiro" << std::endl;
            (*buffer) << "tamanho " << tamanho << std::endl;
            (*buffer) << "tempoTotal " << tempoTotal << std::endl;
            (*buffer) << "direcao " << direcao << std::endl;
        }

        void Vampiro::salvar()
        {
            Inimigo::salvar();
        }
    }
}