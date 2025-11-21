#include "Plataforma.hpp"
#include "../Personagens/Jogador.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Plataforma::Plataforma(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
            Obstaculo(),
            altura((int)size.y)
        {
            danoso = false;
            if (!textura.loadFromFile(caminhoTextura)) 
            {
                std::cerr << "ERRO: Nao foi possivel carregar '" << caminhoTextura << "'" << std::endl;
                exit(1);
            }
            sprite.emplace(textura);
            sprite->setPosition(pos);

            
            // Pega o tamanho original da textura (128x128)
            sf::Vector2f texSize = static_cast<sf::Vector2f>(textura.getSize());
            
            // Calcula a escala necessária para atingir o 'size' desejado
            float scaleX = size.x / texSize.x;
            float scaleY = size.y / texSize.y;

            sprite->setScale({scaleX, scaleY});
            
        }

        Plataforma::~Plataforma() {}

        void Plataforma::executar(float delta)
        {
            velocidade.y -= G_ACCEL.y * delta; 
            
            velocidade.y += G_ACCEL.y * delta;
            if (sprite)
            {
                sprite->move(velocidade * delta);
            }
        }

        void Plataforma::desenhar()
        {
            if (pGG && sprite)
            {
                pGG->desenhar(*sprite);
            }
        }

        sf::FloatRect Plataforma::getBoundingBox() const
        {
            if (sprite)
            {
                sf::FloatRect b = sprite->getGlobalBounds();

                // Expande levemente para tornar a hitbox mais permissiva
                const float EXPAND_X = b.size.x * 0.05f; // 5%
                const float EXPAND_Y = b.size.y * 0.05f; // 5%
                b.position.x -= EXPAND_X / 2.f;
                b.position.y -= EXPAND_Y / 2.f;
                b.size.x += EXPAND_X;
                b.size.y += EXPAND_Y;
                return b;
            }
            return {};
        }
        void Plataforma::obstaculizar(Entidades::Personagens::Jogador* pJogador)
        {
            pJogador->colidir(this, getBoundingBox());
        }
    }
}