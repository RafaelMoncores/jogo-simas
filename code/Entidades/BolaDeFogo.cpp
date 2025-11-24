// BolaDeFogo.cpp
// Implementação do projetil de fogo usado por inimigos e jogadores.
// Usa SFML 3.0.2 para renderização e input. Textura: "tileSets/bola_de_fogo.png".

#include "BolaDeFogo.hpp"
#include "../Gerenciadores/GerenciadorRecursos.hpp"
#include <cmath>
#include <iostream>

namespace Entidades
{

    // Construtor: inicializa direção, velocidade e propriedade (do jogador ou inimigo)
    BolaDeFogo::BolaDeFogo(sf::Vector2f pos, sf::Vector2f dir, float vel, bool doJogador) :
        Entidade(),
        direcao(dir),
        velocidadeProjetil(vel),
        dano(1),
        ativo(true),
        pertenceAoJogador(doJogador)
    {
        // Carrega textura via gerenciador de recursos (centraliza carregamento)
        shape.emplace(Gerenciadores::GerenciadorRecursos::getInstance()->getTextura("tileSets/bola_de_fogo.png"));
        shape->setPosition(pos);

        // Normaliza direção para assegurar velocidade correta
        float mag = std::sqrt(direcao.x * direcao.x + direcao.y * direcao.y);
        if (mag != 0.f)
        {
            direcao /= mag;
        }

        pFig = &(*shape);
    }

    // Destrutor padrão (sem recursos alocados além de optinals)
    BolaDeFogo::~BolaDeFogo() { }

    // Atualiza posição do projetil; desativa se fora da área de jogo
    void BolaDeFogo::executar(float delta)
    {
        if (!shape) return;

        sf::Vector2f pos = shape->getPosition();
        pos += direcao * velocidadeProjetil * delta;
        shape->setPosition(pos);

        // Limites absolutos para limpeza de projéteis
        if (pos.y > 2000.f || pos.x < -500.f || pos.x > 3000.f)
        {
            ativo = false;
        }
    }

    // Renderiza o projetil na tela
    void BolaDeFogo::desenhar()
    {
        if (pGG && shape)
        {
            pGG->desenhar(*shape);
        }
    }

    // Retorna caixa de colisão para checagens de interseção
    sf::FloatRect BolaDeFogo::getBoundingBox() const
    {
        if (shape)
        {
            return shape->getGlobalBounds();
        }
        return {};
    }

    // Se colidir com jogador (quando pertence a inimigo), aplica dano e desativa
    void BolaDeFogo::colidirComJogador(Personagens::Jogador* pJogador)
    {
        if (pJogador && !pertenceAoJogador)
        {
            pJogador->perderVida(dano);
            ativo = false;
        }
    }

    // Getters/Setters simples
    bool BolaDeFogo::getAtivo() const { return ativo; }
    void BolaDeFogo::setAtivo(bool af) { ativo = af; }
    bool BolaDeFogo::getPertenceAoJogador() const { return pertenceAoJogador; }
    int BolaDeFogo::getDano() const { return dano; }

    // Rebate o projetil para o lado oposto e marca como pertencente ao jogador
    void BolaDeFogo::rebater()
    {
        direcao *= -1.f;
        pertenceAoJogador = true;
        if (shape)
        {
            shape->setColor(sf::Color::Cyan);
        }
    }

    // Serializa estado do projetil para snapshot/save
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

} // namespace Entidades
