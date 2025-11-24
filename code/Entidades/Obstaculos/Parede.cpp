#include "Parede.hpp"
#include "../Personagens/Jogador.hpp"
#include "../../Gerenciadores/GerenciadorRecursos.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Parede::Parede(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
            Obstaculo()
        {
            danoso = true;
            
            // 1. Pede a textura ao Gerenciador (se falhar, ele lança o throw para a main)
            sf::Texture& tex = Gerenciadores::GerenciadorRecursos::getInstance()->getTextura(caminhoTextura);
            
            // 2. Garante que essa textura possa se repetir (tilling)
            tex.setRepeated(true);

            // 3. Configura o corpo
            corpo.setPosition(pos);
            corpo.setSize(size);
            
            // Passamos o endereço da textura que está lá no Gerenciador
            corpo.setTexture(&tex);
            
            // 4. Define a área de repetição
            corpo.setTextureRect(sf::IntRect({0, 0}, {(int)size.x, (int)size.y}));
        }

        Parede::Parede(){};

        Parede::~Parede()
        {
        }

        void Parede::executar(float delta)
        {
            velocidade.y -= G_ACCEL.y * delta;
            velocidade.y += G_ACCEL.y * delta;
            corpo.move(velocidade * delta);
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
            sf::FloatRect b = corpo.getGlobalBounds();

            // Expande levemente para evitar "passar pelo canto" em colisões
            const float EXPAND_X = b.size.x * 0.05f;
            const float EXPAND_Y = b.size.y * 0.05f;
            b.position.x -= EXPAND_X / 2.f;
            b.position.y -= EXPAND_Y / 2.f;
            b.size.x += EXPAND_X;
            b.size.y += EXPAND_Y;
            return b;
        }

        void Parede::obstaculizar(Entidades::Personagens::Jogador* pJogador)
        {
            pJogador->colidir(this, getBoundingBox());
            
            if (danoso)
            {
                pJogador->perderVida(1);
            }
        }

        void Parede::salvarDataBuffer()
        {
            Obstaculo::salvarDataBuffer();
            if (!buffer) return;
            (*buffer) << "Parede" << std::endl;
            sf::FloatRect b = getBoundingBox();
            (*buffer) << "posX " << b.position.x << std::endl;
            (*buffer) << "posY " << b.position.y << std::endl;
            (*buffer) << "sizeX " << b.size.x << std::endl;
            (*buffer) << "sizeY " << b.size.y << std::endl;
            (*buffer) << "danoso " << (danoso ? 1 : 0) << std::endl;
        }
    }
}