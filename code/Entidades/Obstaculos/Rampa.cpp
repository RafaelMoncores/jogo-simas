#include "Rampa.hpp"
#include "../Personagens/Jogador.hpp"
#include <iostream>
#include <string>

namespace Entidades
{
    namespace Obstaculos
    {
        Rampa::Rampa(sf::Vector2f pos, sf::Vector2f size, bool sobeEsq, std::string caminhoTextura) :
            Obstaculo(),
            sobeDaEsquerda(sobeEsq),
            largura(size.x)
        {
            danoso = false;
            if (!textura.loadFromFile(caminhoTextura)) 
            {
                std::cerr << "ERRO: Nao foi possivel carregar '" << caminhoTextura << "'" << std::endl;
                exit(1);
            }
            sprite.emplace(textura);
            sprite->setPosition(pos);

            sf::Vector2f texSize = static_cast<sf::Vector2f>(textura.getSize());
            float scaleX = size.x / texSize.x;
            float scaleY = size.y / texSize.y;

            if (!sobeDaEsquerda)
            {
                scaleX *= -1.f;
                sprite->setPosition({pos.x + size.x, pos.y});
            }
            
            sprite->setScale({scaleX, scaleY});
        }

        Rampa::~Rampa() {}

        void Rampa::desenhar()
        {
            if (pGG && sprite)
            {
                pGG->desenhar(*sprite);
            }
        }

        sf::FloatRect Rampa::getBoundingBox() const
        {
            if (sprite)
            {
                // Retorna o retângulo que contém a rampa
                return sprite->getGlobalBounds();
            }
            return {};
        }

        bool Rampa::getSobeDaEsquerda() const
        {
            return sobeDaEsquerda;
        }

        void Rampa::obstaculizar(Entidades::Personagens::Jogador* pJogador)
        {
            pJogador->colidir(this, getBoundingBox());
        }
    }
}