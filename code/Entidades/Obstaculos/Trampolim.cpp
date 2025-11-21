#include "Trampolim.hpp"
#include "../Personagens/Jogador.hpp"
#include <cmath>
#include <iostream>

namespace Entidades
{
    namespace Obstaculos
    {
        Trampolim::Trampolim(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura, float forca) :
            Obstaculo(),
            FORCA_REBOTE(forca)
        {
            danoso = false;

            // Lógica do corpo (pode copiar/adaptar da Parede)
            if (!textura.loadFromFile(caminhoTextura))
            {
                std::cerr << "ERRO: Nao foi possivel carregar '" << caminhoTextura << "'" << std::endl;
            }
            textura.setRepeated(true);
            corpo.setPosition(pos);
            corpo.setSize(size);
            corpo.setTexture(&textura);
            corpo.setTextureRect(sf::IntRect({0, 0}, {(int)size.x, (int)size.y}));
        }

        Trampolim::~Trampolim() {}

        void Trampolim::executar(float delta) 
        { 
            velocidade.y -= G_ACCEL.y * delta;
            velocidade.y += G_ACCEL.y * delta;
            corpo.move(velocidade * delta);
        }

        void Trampolim::desenhar()
        {
            if (pGG) pGG->desenhar(corpo);
        }

        sf::FloatRect Trampolim::getBoundingBox() const
        {
            sf::FloatRect b = corpo.getGlobalBounds();

            // Pequena expansão para tornar a hitbox mais alinhada ao visual
            const float EXPAND_X = b.size.x * 0.05f;
            const float EXPAND_Y = b.size.y * 0.05f;
            b.position.x -= EXPAND_X / 2.f;
            b.position.y -= EXPAND_Y / 2.f;
            b.size.x += EXPAND_X;
            b.size.y += EXPAND_Y;
            return b;
        }

        // --- A LÓGICA PRINCIPAL ---
        void Trampolim::obstaculizar(Entidades::Personagens::Jogador* pJogador)
        {
            // 1. Pega as hitboxes
            sf::FloatRect boundsJogador = pJogador->getBoundingBox();
            sf::FloatRect boundsTrampolim = getBoundingBox();

            // 2. Calcula Overlaps (lógica de Jogador::colidir)
            sf::Vector2f centroJogador(boundsJogador.position.x + boundsJogador.size.x / 2.f, boundsJogador.position.y + boundsJogador.size.y / 2.f);
            sf::Vector2f centroTrampolim(boundsTrampolim.position.x + boundsTrampolim.size.x / 2.f, boundsTrampolim.position.y + boundsTrampolim.size.y / 2.f);
            sf::Vector2f distCentros(centroJogador.x - centroTrampolim.x, centroJogador.y - centroTrampolim.y);
            
            float somaMeiasLarguras = boundsJogador.size.x / 2.f + boundsTrampolim.size.x / 2.f;
            float somaMeiasAlturas = boundsJogador.size.y / 2.f + boundsTrampolim.size.y / 2.f;
            
            float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
            float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

            // Inicia a força de repulsão (baseada na velocidade atual)
            sf::Vector2f forcaRepel = pJogador->getVelocidade();

            // 3. Decide a direção do rebote
            if (overlapY < overlapX)
            {
                // Colisão Vertical
                if (distCentros.y > 0)
                {
                    // Bateu no FUNDO do trampolim -> Rebate para BAIXO
                    forcaRepel.y = FORCA_REBOTE;
                    pJogador->colidir(this, boundsTrampolim); // Aplica física da batida na cabeça
                }
                else
                {
                    // Bateu no TOPO do trampolim -> Rebate para CIMA
                    forcaRepel.y = -FORCA_REBOTE;
                }
            }
            else
            {
                // Colisão Horizontal
                if (distCentros.x > 0) 
                {
                    // Bateu na ESQUERDA do trampolim -> Rebate para DIREITA
                    forcaRepel.x = FORCA_REBOTE;
                }
                else 
                {
                    // Bateu na DIREITA do trampolim -> Rebate para ESQUERDA
                    forcaRepel.x = -FORCA_REBOTE;
                }
                pJogador->colidir(this, boundsTrampolim); // Aplica física da batida lateral
            }

            // 4. Aplica o rebote
            pJogador->aplicarRepel(forcaRepel);
        }
    }
}