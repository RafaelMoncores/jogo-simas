#include "Dragao.hpp"
#include "../BolaDeFogo.hpp"
#include "../Obstaculos/Obstaculo.hpp"
#include "../../Gerenciadores/GerenciadorRecursos.hpp"
#include <iostream>
#include <cmath>


namespace Entidades
{
    namespace Personagens
    {
        // Construtor: inicializa o Dragao com timers, estado de ataque e sprite
        Dragao::Dragao(sf::Vector2f pos, Jogador* pJog1, Jogador* pJog2, Listas::ListaEntidades* pLista) :
            Inimigo(9, pos, pJog1, pJog2),
            forca(2),
            pListaEntidades(pLista),
            estaAtacando(false),
            temporizadorMovimento(0.f),
            temporizadorAtaque(5.f),
            cooldownTiro(0.f),
            tirosDisparados(0),
            tempoMovimentoRestante(0.f),
            ignorandoObstaculos(false)
        {
            // Carrega textura do dragão (verificar licença das texturas em tileSets/)
            sprite.emplace(Gerenciadores::GerenciadorRecursos::getInstance()->getTextura("tileSets/dragao.png"));
            sprite->setPosition(posInicial);
            sprite->setScale({3, 3});

            nivel_maldade = 3;
        }

        Dragao::~Dragao()
        {
        }

        // Gera e adiciona um projetil direcionado ao jogador mais próximo
        void Dragao::atirar()
        {
            Jogador* pAlvo = getJogadorMaisProximo();
            if (!pAlvo || !pListaEntidades || !sprite) return;

            sf::Vector2f posDragao = sprite->getPosition();
            posDragao.x += sprite->getGlobalBounds().size.x / 2.f;
            posDragao.y += sprite->getGlobalBounds().size.y / 2.f;

            sf::FloatRect boundsJog = pAlvo->getBoundingBox();
            sf::Vector2f posJogador(
                boundsJog.position.x + boundsJog.size.x / 2.f,
                boundsJog.position.y + boundsJog.size.y / 2.f
            );

            sf::Vector2f direcao = posJogador - posDragao;

            BolaDeFogo* pFogo = new BolaDeFogo(posDragao, direcao, 400.f, false);
            pListaEntidades->incluir(static_cast<Entidade*>(pFogo));
        }

        // Atualiza lógica do Dragao: movimento, ataques e timers
        void Dragao::executar(float delta)
        {
            if (num_vidas <= 0)
            {
                if (sprite)
                {
                    sprite.reset();
                }
                return;
            }

            temporizadorMovimento += delta;
            temporizadorAtaque += delta;

            Jogador* pAlvo = getJogadorMaisProximo();

            if (estaAtacando)
            {
                // Estado de ataque: para de andar e dispara projéteis em rajada
                velocidade.x = 0.f;
                ignorandoObstaculos = false;
                tempoMovimentoRestante = 0.f;

                cooldownTiro += delta;
                if (cooldownTiro >= 1.0f && tirosDisparados < 3)
                {
                    atirar();
                    cooldownTiro = 0.f;
                    tirosDisparados++;
                }

                if (tirosDisparados >= 3)
                {
                    estaAtacando = false;
                    tirosDisparados = 0;
                    temporizadorAtaque = 0.f;
                }
            }
            else
            {
                // Estado de patrulha / preparação: alterna entre andar e atacar
                if (temporizadorAtaque >= 8.0f)
                {
                    estaAtacando = true;
                    cooldownTiro = 1.0f;
                    tirosDisparados = 0;
                    temporizadorMovimento = 0.f;
                }

                if (tempoMovimentoRestante <= 0.f && temporizadorMovimento >= 5.0f)
                {
                    if (pAlvo && sprite)
                    {
                        sf::Vector2f posJogador = pAlvo->getBoundingBox().position;
                        sf::Vector2f posDragao = sprite->getPosition();

                        if (posJogador.x < posDragao.x)
                            velocidade.x = -150.f;
                        else
                            velocidade.x = 150.f;

                        ignorandoObstaculos = true;
                        tempoMovimentoRestante = 1.5f;
                    }
                    temporizadorMovimento = 0.f;
                }

                if (tempoMovimentoRestante > 0.f)
                {
                    tempoMovimentoRestante -= delta;
                    if (tempoMovimentoRestante <= 0.f)
                    {
                        velocidade.x = 0.f;
                        ignorandoObstaculos = false;
                    }
                }
            }

            aplicarFisica(delta);
        }
        

        // Aplica física simples: gravidade e movimento; verifica 'death plane'
        void Dragao::aplicarFisica(float delta)
        {
            if (sprite)
            {
                velocidade.y += G_ACCEL.y * delta;

                // (redundante: subtrai novamente para manter equilíbrio de sinais)
                velocidade.y -= G_ACCEL.y * delta;
                sprite->move(velocidade * delta);

                float deathPlaneY = 1500.f;
                if (sprite->getPosition().y > deathPlaneY)
                {
                    perderVida(num_vidas);
                }
            }
        }
        
        // Quando danifica outro personagem (a lógica do dragão causa dano ao jogador)
        void Dragao::danificar(Personagem* pOutro)
        {
            if (num_vidas <= 0) return;
            pOutro->perderVida(forca);
        }

        void Dragao::colidir(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            Obstaculos::Obstaculo* pObst = dynamic_cast<Obstaculos::Obstaculo*>(pOutra);
            
            if (pObst && ignorandoObstaculos)
            {
                return;
            }

            if (num_vidas <= 0) return;
            if (!sprite) return;
            sf::FloatRect boundsPropria = getBoundingBox();

            sf::Vector2f centroProprio(boundsPropria.position.x + boundsPropria.size.x / 2.f, boundsPropria.position.y + boundsPropria.size.y / 2.f);
            sf::Vector2f centroOutra(boundsOutra.position.x + boundsOutra.size.x / 2.f, boundsOutra.position.y + boundsOutra.size.y / 2.f);
            sf::Vector2f distCentros(centroProprio.x - centroOutra.x, centroProprio.y - centroOutra.y);
            float somaMeiasLarguras = boundsPropria.size.x / 2.f + boundsOutra.size.x / 2.f;
            float somaMeiasAlturas = boundsPropria.size.y / 2.f + boundsOutra.size.y / 2.f;
            float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
            float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

            if (overlapX <= 0 || overlapY <= 0) return;

            if (overlapY < overlapX)
            {
                if (distCentros.y > 0)
                { 
                    sprite->move({ 0.f, overlapY }); 
                }
                else
                { 
                    sprite->move({ 0.f, -overlapY }); 
                    if (velocidade.y >= 0)
                    {
                        podePular = true;
                        velocidade.y = 0.f;
                    }
                }
            }
            else
            {
                if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                else { sprite->move({ -overlapX, 0.f }); }
                velocidade.x = 0.f;
            }
        }

        void Dragao::salvarDataBuffer()
        {
            Inimigo::salvarDataBuffer();
            if (!buffer) return;
            (*buffer) << "Dragao" << std::endl;
            (*buffer) << "forca " << forca << std::endl;
            (*buffer) << "estaAtacando " << (estaAtacando ? 1 : 0) << std::endl;
            (*buffer) << "temporizadorMovimento " << temporizadorMovimento << std::endl;
            (*buffer) << "temporizadorAtaque " << temporizadorAtaque << std::endl;
            (*buffer) << "tirosDisparados " << tirosDisparados << std::endl;
            (*buffer) << "tempoMovimentoRestante " << tempoMovimentoRestante << std::endl;
        }

        void Dragao::salvar()
        {
            Inimigo::salvar();
        }
    }
}