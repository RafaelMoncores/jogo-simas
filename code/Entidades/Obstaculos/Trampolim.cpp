#include "Trampolim.hpp"
#include "../Personagens/Jogador.hpp"
#include <iostream>
#include <string>

namespace Entidades
{
    namespace Obstaculos
    {
        Trampolim::Trampolim(sf::Vector2f pos, sf::Vector2f size, bool sobeEsq, std::string caminhoTextura) :
            Obstaculo(),
            sobeDaEsquerda(sobeEsq),
            largura(size.x),
            m_pos(pos),
            m_size(size)
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

        Trampolim::~Trampolim() {}

        void Trampolim::desenhar()
        {
            if (pGG && sprite)
            {
                pGG->desenhar(*sprite);
            }
        }

        sf::FloatRect Trampolim::getBoundingBox() const
        {
            return sf::FloatRect(m_pos, m_size);
        }

        bool Trampolim::getSobeDaEsquerda() const
        {
            return sobeDaEsquerda;
        }

        void Trampolim::obstaculizar(Entidades::Personagens::Jogador* pJogador)
        {
            pJogador->colidir(this, getBoundingBox());
        }
    }
}