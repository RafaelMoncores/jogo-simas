#include "BolaDeFogo.hpp"
#include <cmath>
#include <iostream>

namespace Entidades
{
    BolaDeFogo::BolaDeFogo(sf::Vector2f pos, sf::Vector2f dir, float vel) :
        Entidade(),
        direcao(dir),
        velocidadeProjetil(vel),
        dano(1),
        ativo(true)
    {
        if (!textura.loadFromFile("tileSets/bola_de_fogo.png"))
        {
            std::cerr << "ERRO FATAL: Nao foi possivel carregar 'bola_de_fogo.png'" << std::endl;
            exit(1);
        }
        
        shape.emplace(textura);
        shape->setPosition(pos);
        float mag = std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y);
        if (mag != 0.f)
        {
            direcao /= mag;
        }
        
        pFig = &(*shape);
    }

    BolaDeFogo::~BolaDeFogo()
    {
    }

    void BolaDeFogo::executar(float delta)
    {
        if (!shape) return;

        sf::Vector2f pos = shape->getPosition();
        pos += direcao * velocidadeProjetil * delta;
        shape->setPosition(pos);

        if (pos.y > 2000.f || pos.x < -500.f || pos.x > 3000.f)
        {
            ativo = false;
        }
    }

    void BolaDeFogo::desenhar()
    {
        // pGG é o GerenciadorGrafico estático de Ente
        if (pGG && shape) 
        {
            pGG->desenhar(*shape);
        }
    }

    sf::FloatRect BolaDeFogo::getBoundingBox() const
    {
        if (shape)
        {
            return shape->getGlobalBounds();
        }
        return {};
    }

    void BolaDeFogo::colidirComJogador(Personagens::Jogador* pJogador)
    {
        if (pJogador)
        {
            pJogador->perderVida(dano);
            ativo = false;
        }
    }

    bool BolaDeFogo::getAtivo() const
    {
        return ativo;
    }

}