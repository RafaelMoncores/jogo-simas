#include "Jogo.hpp"
#include <iostream>

Jogo::Jogo() :
    gerenciadorGrafico(),
    jogador1(),
    relogio()
{
    // --- ADICIONE ESTA CONFIGURAÇÃO ---
    testeQuadrado.setSize(sf::Vector2f(100.f, 100.f)); // Tamanho 100x100
    testeQuadrado.setPosition(sf::Vector2f(50.f, 50.f)); // Posição (50, 50)
    // --- FIM DA ADIÇÃO ---
}

Jogo::~Jogo()
{
}

void Jogo::executar()
{
    while (gerenciadorGrafico.isWindowOpen())
    {
        processarEventos();
        atualizar();
        renderizar();
    }
}

void Jogo::processarEventos()
{
    while (auto event = gerenciadorGrafico.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            gerenciadorGrafico.fecharWindow();
        }
    }
}

void Jogo::atualizar()
{
    jogador1.atualizar();
}

// Lógica de desenho
void Jogo::renderizar()
{
    std::cout << "Renderizando frame..." << std::endl;

    gerenciadorGrafico.limpar();

    // --- ADICIONE O DESENHO DO QUADRADO ---
    std::cout << "  -> Desenhando QUADRADO..." << std::endl;
    gerenciadorGrafico.desenhar(testeQuadrado);
    // --- FIM DA ADIÇÃO ---

    jogador1.desenhar(gerenciadorGrafico); // Esta linha já estava aqui
    gerenciadorGrafico.exibir();
}