#include "Plataforma.hpp"
#include "../Personagens/Jogador.hpp"
#include "../../Gerenciadores/GerenciadorRecursos.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Plataforma::Plataforma(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
            Obstaculo()
        {
            danoso = false;

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

        Plataforma::~Plataforma() {}

        void Plataforma::executar(float delta)
        {
            // Aplica gravidade (se necessário, embora plataformas costumem ser estáticas)
            velocidade.y -= G_ACCEL.y * delta; 
            velocidade.y += G_ACCEL.y * delta;
            
            // Move o retângulo
            corpo.move(velocidade * delta);
        }

        void Plataforma::desenhar()
        {
            if (pGG)
            {
                pGG->desenhar(corpo);
            }
        }

        sf::FloatRect Plataforma::getBoundingBox() const
        {
            return corpo.getGlobalBounds();
        }

        void Plataforma::obstaculizar(Entidades::Personagens::Jogador* pJogador)
        {
            pJogador->colidir(this, getBoundingBox());
        }

        void Plataforma::salvarDataBuffer()
        {
            Obstaculo::salvarDataBuffer();
            if (!buffer) return;
            (*buffer) << "Plataforma" << std::endl;
            sf::FloatRect b = getBoundingBox();
            (*buffer) << "posX " << b.position.x << std::endl;
            (*buffer) << "posY " << b.position.y << std::endl;
            (*buffer) << "sizeX " << b.size.x << std::endl;
            (*buffer) << "sizeY " << b.size.y << std::endl;
        }
    }
}