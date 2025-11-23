#include "BolaDeFogo.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

namespace Entidades
{
    BolaDeFogo::BolaDeFogo(sf::Vector2f pos, sf::Vector2f dir, float vel, bool doJogador) :
        Entidade(),
        direcao(dir),
        velocidadeProjetil(vel),
        dano(1),
        ativo(true),
        pertenceAoJogador(doJogador)
    {
        if (!textura.loadFromFile("tileSets/bola_de_fogo.png"))
        {
            throw std::runtime_error("Erro: Nao foi possivel carregar a textura: 'bola_de_fogo.png'");
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
        if (pJogador && !pertenceAoJogador)
        {
            pJogador->perderVida(dano);
            ativo = false;
        }
    }

    bool BolaDeFogo::getAtivo() const
    {
        return ativo;
    }

    void BolaDeFogo::rebater()
    {
        direcao *= -1.f; 
        pertenceAoJogador = true;
        
        if (shape)
        {
            shape->setColor(sf::Color::Cyan); 
        }
    }

    void BolaDeFogo::setAtivo(bool af)
    {
        ativo = af;
    }

    bool BolaDeFogo::getPertenceAoJogador() const
    {
        return pertenceAoJogador;
    }

    int BolaDeFogo::getDano() const
    {
        return dano;
    }

    void BolaDeFogo::salvarDataBuffer()
    {
        Entidade::salvarDataBuffer();
        if (!buffer) return;
        (*buffer) << "BolaDeFogo" << std::endl;
        (*buffer) << "dirX " << direcao.x << std::endl;
        (*buffer) << "dirY " << direcao.y << std::endl;
        (*buffer) << "velProjetil " << velocidadeProjetil << std::endl;
        (*buffer) << "dano " << dano << std::endl;
        (*buffer) << "ativo " << (ativo ? 1 : 0) << std::endl;
        (*buffer) << "pertenceAoJogador " << (pertenceAoJogador ? 1 : 0) << std::endl;
    }

}