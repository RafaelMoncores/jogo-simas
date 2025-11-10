#include "Jogador.hpp"
#include "../Obstaculos/Plataforma.hpp"
#include "../Obstaculos/Rampa.hpp"
#include <iostream>
#include <stdlib.h>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>

namespace Entidades
{
    namespace Personagens
    {

        Jogador::Jogador(sf::Vector2f pos) :
            Personagem(3, pos),
            FORCA_PULO(420.0f),
            MULTIPLICADOR_PULO_CURTO(3.0f),
            estaNaRampa(false),
            rampaSobeEsquerda(false),
            direcao(1),
            estaAtacando(false),
            tempoAtaque(0.0f),
            COOLDOWN_ATAQUE(0.3f)
        {
            if (!textura.loadFromFile("player.png"))
            {
                std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
                exit(1);
            }
            
            sprite.emplace(textura); 
            sprite->setPosition(posInicial);
        }

        Jogador::~Jogador()
        {
        }

        void Jogador::processarInputs(float delta)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && podePular)
            {
                velocidade.y = -FORCA_PULO;
                podePular = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !estaAtacando)
            {
                estaAtacando = true;
                tempoAtaque = COOLDOWN_ATAQUE;
                // (TODO: mudar para a animação do sprite para "atacando")
            }
        }

        void Jogador::aplicarFisica(float delta)
        {
            // --- FATOR DE LENTIDÃO ---
            const float FATOR_LENTIDAO_RAMPA = 0.15f; // (Pode testar com 0.0f agora)
            float modificadorAceleracao = 0.7f; 

            // 1. INPUT HORIZONTAL
            bool inputEsquerda = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
            bool inputDireita = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
            bool noInputHorizontal = !inputEsquerda && !inputDireita;

            // --- LÓGICA DE LENTIDÃO (Correta) ---
            if (estaNaRampa)
            {
                // Se estiver a tentar subir...
                if ((rampaSobeEsquerda && inputDireita) || // Subindo a rampa '/'
                    (!rampaSobeEsquerda && inputEsquerda))  // Subindo a rampa '\'
                {
                    // ...aplique a lentidão.
                    modificadorAceleracao = FATOR_LENTIDAO_RAMPA;
                }
            }

            // Aplica o input de aceleração, JÁ MODIFICADO pelo fator de lentidão
            if (inputEsquerda)
            {
                velocidade.x -= ACELERACAO_LATERAL * modificadorAceleracao * delta;
            }
            if (inputDireita)
            {
                velocidade.x += ACELERACAO_LATERAL * modificadorAceleracao * delta;
            }

            // 2. LÓGICA DO PULO CURTO (Sem alteração)
            if (velocidade.y < 0.0f && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
            {
                velocidade.y += G_ACCEL.y * MULTIPLICADOR_PULO_CURTO * delta;
            }

            // 3. GRAVIDADE (Sem alteração)
            velocidade.y += G_ACCEL.y * delta;

            
            // 4. LÓGICA DE FRICÇÃO (Atrito) E SLIDE (Deslizamento) - CORRIGIDA
            
            if (estaNaRampa) 
            {
                // ESTAMOS NA RAMPA
                const float FORCA_SLIDE_RAMPA = 20.0f; // O seu valor de slide

                // SÓ APLIQUE O SLIDE SE O JOGADOR ESTIVER PARADO
                if (noInputHorizontal)
                {
                    // Rampa '/' (sobe da esquerda) -> desliza para a ESQUERDA (negativo)
                    if (rampaSobeEsquerda) 
                    {
                        velocidade.x -= FORCA_SLIDE_RAMPA * delta; // <--- CORRIGIDO
                    }
                    // Rampa '\' (sobe da direita) -> desliza para a DIREITA (positivo)
                    else 
                    {
                        velocidade.x += FORCA_SLIDE_RAMPA * delta; // <--- CORRIGIDO
                    }
                }
            }
            else if (podePular && noInputHorizontal)
            {
                // ESTAMOS EM CHÃO PLANO (lógica de fricção original)
                if (std::abs(velocidade.x) > 0.1f)
                {
                    if (velocidade.x > 0.0f)
                        velocidade.x -= FRICCAO_LATERAL * delta;
                    else
                        velocidade.x += FRICCAO_LATERAL * delta;
                }
                if (std::abs(velocidade.x) < 0.5f)
                {
                    velocidade.x = 0.0f;
                }
            }

            // 5. LIMITAR VELOCIDADE E MOVER (Sem alteração)
            velocidade.x = std::clamp(velocidade.x, -VELOCIDADE_MAXIMA_LATERAL, VELOCIDADE_MAXIMA_LATERAL);
            
            if (sprite)
            {
                sprite->move(velocidade * delta);
                
                float deathPlaneY = 1500.f;
                if (sprite->getPosition().y > deathPlaneY)
                {
                    perderVida(num_vidas);
                }
            }
        }

        void Jogador::executar(float delta)
        {
            if (estaAtacando)
            {
                tempoAtaque -= delta;
                if (tempoAtaque <= 0.0f)
                {
                    estaAtacando = false;
                    // (TODO: Mudar animação de volta para "idle" ou "walk")
                }
            }
            processarInputs(delta);
            aplicarFisica(delta);
        }

        bool Jogador::getEstaAtacando() const
        {
            return estaAtacando;
        }

        int Jogador::getDirecao() const
        {
            return direcao;
        }

        sf::FloatRect Jogador::getBoundingBox() const
        {
            if (sprite)
            {
                sf::FloatRect bounds = sprite->getGlobalBounds();
                
                float ajusteLargura = 48.0f;
                bounds.position.x += ajusteLargura / 2.f;
                bounds.size.x -= ajusteLargura;
                float ajusteAltura = 10.0f;
                bounds.size.y -= ajusteAltura;

                return bounds;
            }
            return {};
        }

        void Jogador::resolverColisao(Entidade* pOutra, sf::FloatRect boundsOutra)
        {
            if (!sprite) return;
            sf::FloatRect boundsPropria = getBoundingBox();


            sf::Vector2f posAtual = sprite->getPosition();
            sf::Vector2f centroProprio(boundsPropria.position.x + boundsPropria.size.x / 2.f, boundsPropria.position.y + boundsPropria.size.y / 2.f);

            sf::Vector2f centroOutra(boundsOutra.position.x + boundsOutra.size.x / 2.f, boundsOutra.position.y + boundsOutra.size.y / 2.f);
            sf::Vector2f distCentros(centroProprio.x - centroOutra.x, centroProprio.y - centroOutra.y);

            float somaMeiasLarguras = boundsPropria.size.x / 2.f + boundsOutra.size.x / 2.f;
            float somaMeiasAlturas = boundsPropria.size.y / 2.f + boundsOutra.size.y / 2.f;
            float overlapX = somaMeiasLarguras - std::abs(distCentros.x);
            float overlapY = somaMeiasAlturas - std::abs(distCentros.y);

            if (overlapX <= 0 || overlapY <= 0) return;

            if (auto* pRampa = dynamic_cast<Obstaculos::Rampa*>(pOutra)){
                float peY = boundsPropria.position.y + boundsPropria.size.y;
                float peX_esq = boundsPropria.position.x;
                float peX_dir = boundsPropria.position.x + boundsPropria.size.x;
                auto calcularAlturaRampa = [&](float pontoX) -> float {

                    float xRelativo = pontoX - boundsOutra.position.x;
                    float pctRampa = std::clamp(xRelativo / boundsOutra.size.x, 0.f, 1.f);
                    float alturaNoPonto;

                    if (pRampa->getSobeDaEsquerda()){
                        alturaNoPonto = boundsOutra.size.y * (1.f - pctRampa);
                    }

                    else{
                        alturaNoPonto = boundsOutra.size.y * pctRampa;
                    }
                    return boundsOutra.position.y + alturaNoPonto;
                };

                float yTopoRampa = calcularAlturaRampa(centroProprio.x);
                float overlapRampa = peY - yTopoRampa;

                float stepUpTolerance = 2.f; // 2 pixels de tolerância
                if (overlapRampa > -stepUpTolerance && velocidade.y >= 0){
                    sprite->move({ 0.f, -overlapRampa });
                    velocidade.y = 0.f;
                    podePular = true;

                    estaNaRampa = true;
                    rampaSobeEsquerda = pRampa->getSobeDaEsquerda();
                }
                else{
                    if (overlapY < overlapX){
                        if (distCentros.y > 0){
                            sprite->move({ 0.f, overlapY });
                            velocidade.y = 0.f;
                        }
                    }
                    else{
                        if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                        else { sprite->move({ -overlapX, 0.f }); }
                        velocidade.x = 0.f;
                    }
                }
            }
            else{
                if (overlapY < overlapX){
                    if (distCentros.y > 0){
                        sprite->move({ 0.f, overlapY });
                        velocidade.y = 0.f;
                    }
                    else{
                        if (velocidade.y >= 0){
                            sprite->move({ 0.f, -overlapY });
                            velocidade.y = 0.f;
                            podePular = true;
                            estaNaRampa = false;
                        }
                    }
                }
                else{
                    float peJogador = boundsPropria.position.y + boundsPropria.size.y;
                    float topoPlataforma = boundsOutra.position.y;

                    if (peJogador <= topoPlataforma + 1.f) { /* Ignora (quina) */ }
                    else{
                        if (distCentros.x > 0) { sprite->move({ overlapX, 0.f }); }
                        else { sprite->move({ -overlapX, 0.f }); }
                        velocidade.x = 0.f;
                    }
                }
            }
        }
    }
}