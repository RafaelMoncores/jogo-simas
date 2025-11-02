#include "GerenciadorColisoes.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace Gerenciadores
{
    GerenciadorColisoes::GerenciadorColisoes() {}
    GerenciadorColisoes::~GerenciadorColisoes() {}

    void GerenciadorColisoes::verificarColisoes(
        Entidades::Personagens::Jogador* pJogador,
        std::list<Entidades::Obstaculos::Obstaculo*>* pObstaculos) // <-- DE Plataforma PARA Obstaculo
    {
        if (!pJogador) return;

        sf::FloatRect boundsJogador = pJogador->getBoundingBox();
        pJogador->setPodePular(false); // Assumir que não pode pular

        for (auto* pObst : *pObstaculos) // 
        {
            sf::FloatRect boundsObst = pObst->getBoundingBox();


            bool colidindo =
                // A está à esquerda de B?
                (boundsJogador.position.x < boundsObst.position.x + boundsObst.size.x) &&
                // A está à direita de B?
                (boundsJogador.position.x + boundsJogador.size.x > boundsObst.position.x) &&
                // A está acima de B?
                (boundsJogador.position.y < boundsObst.position.y + boundsObst.size.y) &&
                // A está abaixo de B?
                (boundsJogador.position.y + boundsJogador.size.y > boundsObst.position.y);
            
            
            if (colidindo)
            {
                // Teve colisão, agora precisamos resolver
                pJogador->resolverColisao(pObst, boundsObst);
            }
            
        }
    }
}