#include "Plataforma.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Plataforma::Plataforma(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
            Obstaculo()
        {
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

        // Plataformas estáticas não fazem nada no executar
        void Plataforma::executar(float delta)
        {
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
                return sprite->getGlobalBounds();
            }
            return {};
        }
    }
}