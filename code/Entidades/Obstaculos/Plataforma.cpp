#include "Plataforma.hpp"
#include "../Personagens/Jogador.hpp"
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Plataforma::Plataforma(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
            Obstaculo()
        {
            danoso = false;
            
            // 1. Carrega a textura
            if (!textura.loadFromFile(caminhoTextura)) 
            {
                std::cerr << "ERRO: Nao foi possivel carregar '" << caminhoTextura << "'" << std::endl;
                exit(1);
            }

            // 2. Ativa a repetição (O Segredo do "Azulejo")
            textura.setRepeated(true);

            // 3. Configura o corpo retangular
            corpo.setPosition(pos);
            corpo.setSize(size);
            corpo.setTexture(&textura);
            
            // 4. Define a área da textura igual ao tamanho do objeto
            // Isso faz com que a textura se repita em vez de esticar
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