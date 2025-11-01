#include "Jogador.hpp"
#include <iostream>
#include <stdlib.h>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm> // <-- ADICIONADO para std::clamp
#include <cmath>     // <-- ADICIONADO para std::abs

Jogador::Jogador() :
    Entidade(),
    // --- MUDANÇA: Novas Constantes ---
    VELOCIDADE_MAXIMA_LATERAL(350.0f),
    ACELERACAO_LATERAL(900.0f),       // Acelera rápido
    FRICCAO_LATERAL(1200.0f),         // Para rápido
    FORCA_PULO(420.0f),
    MULTIPLICADOR_PULO_CURTO(3.0f),
    podePular(false)
    // REMOVIDO: travadoNoAr(false)
{
    if (!textura.loadFromFile("player.png"))
    {
        std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
        exit(1);
    }

    sprite.emplace(textura);
    sprite->setPosition({400.0f, 300.0f}); 
}

Jogador::~Jogador()
{
}

void Jogador::processarInputs(float delta) // <-- RECEBE DELTA
{
    // --- 1. Movimento Horizontal (ACELERAÇÃO) ---
    // Em vez de SETAR a velocidade, nós ACELERAMOS
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
    {
        velocidade.x -= ACELERACAO_LATERAL * delta;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        velocidade.x += ACELERACAO_LATERAL * delta;
    }

    // --- 2. Lógica do Pulo (SEM MUDANÇA) ---
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && podePular)
    {
        velocidade.y = -FORCA_PULO; 
        podePular = false;
        // REMOVIDO: Lógica do 'travadoNoAr'
    }
}

void Jogador::aplicarFisica(float delta) // <-- RENOMEADO
{
    if (sprite) 
    {
        // --- 1. Pulo Variável (Gravidade) ---
        // (Lógica anterior, sem mudança)
        if (velocidade.y < 0.0f && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            velocidade.y += G_ACCEL.y * MULTIPLICADOR_PULO_CURTO * delta;
        }
        else
        {
            velocidade.y += G_ACCEL.y * delta;
        }

        // --- 2. Fricção Horizontal (DESACELERAÇÃO) ---
        bool noInputHorizontal = !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && 
                               !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

        // Só aplica fricção no chão (sensação clássica de plataforma)
        if (podePular && noInputHorizontal)
        {
            if (std::abs(velocidade.x) > 0.1f)
            {
                // Aplica força de fricção na direção oposta
                if (velocidade.x > 0.0f)
                    velocidade.x -= FRICCAO_LATERAL * delta;
                else
                    velocidade.x += FRICCAO_LATERAL * delta;
            }
            // Se a velocidade for muito pequena, zera (para evitar "deslizar" para sempre)
            if (std::abs(velocidade.x) < 0.5f)
            {
                velocidade.x = 0.0f;
            }
        }
        
        // --- 3. Limite de Velocidade (Clamp) ---
        velocidade.x = std::clamp(velocidade.x, -VELOCIDADE_MAXIMA_LATERAL, VELOCIDADE_MAXIMA_LATERAL);


        // --- 4. Aplica o movimento final ---
        sprite->move(velocidade * delta);

        // --- 5. Lógica do Chão Falso ---
        float chaoFalso = 800.0f;
        sf::Vector2f pos = sprite->getPosition();

        if (pos.y > chaoFalso)
        {
            sprite->setPosition({pos.x, chaoFalso});
            velocidade.y = 0.0f;
            podePular = true; 
        }
    }
}

void Jogador::executar(float delta)
{
    processarInputs(delta);
    aplicarFisica(delta);
}

void Jogador::desenhar()
{
    if (pGG && sprite)
    {
        pGG->desenhar(*sprite);
    }
}