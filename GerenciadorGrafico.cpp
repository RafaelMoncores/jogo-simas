#include "GerenciadorGrafico.hpp"

GerenciadorGrafico::GerenciadorGrafico() : 
    window(sf::VideoMode({1200, 1400}), "Meu Jogo SFML!")
{
}

GerenciadorGrafico::~GerenciadorGrafico() 
{
}

bool GerenciadorGrafico::isWindowOpen() const
{
    return window.isOpen();
}

void GerenciadorGrafico::fecharWindow()
{
    window.close();
}

std::optional<sf::Event> GerenciadorGrafico::pollEvent()
{
    return window.pollEvent();
}

void GerenciadorGrafico::limpar(sf::Color cor)
{
    window.clear(cor);
}

void GerenciadorGrafico::desenhar(const sf::Drawable& desenhavel)
{
    window.draw(desenhavel);
}

void GerenciadorGrafico::exibir()
{
    window.display();
}