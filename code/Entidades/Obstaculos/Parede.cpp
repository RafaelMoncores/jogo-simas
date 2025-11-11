#include "Parede.hpp"
#include "../Personagens/Jogador.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Parede::Parede(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
            Obstaculo()
        {
            danoso = false;
            if (!textura.loadFromFile(caminhoTextura))
            {
                std::cerr << "ERRO FATAL: Nao foi possivel carregar '" << caminhoTextura << "'" << std::endl;
                exit(1);
            }
            textura.setRepeated(true);

            corpo.setPosition(pos);
            corpo.setSize(size);
            corpo.setTexture(&textura);
            corpo.setTextureRect(sf::IntRect({0, 0}, {(int)size.x, (int)size.y}));
        }

        Parede::Parede(){};

        Parede::~Parede()
        {
        }

        void Parede::executar(float delta)
        {
        }

        void Parede::desenhar()
        {
            if (pGG)
            {
                pGG->desenhar(corpo);
            }
        }

        sf::FloatRect Parede::getBoundingBox() const
        {
            return corpo.getGlobalBounds();
        }

        void Parede::obstaculizar(Personagens::Jogador* pJogador)
        {
            return;
        }
    }
}