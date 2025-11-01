#include "GerenciadorGrafico.hpp"

GerenciadorGrafico* GerenciadorGrafico::pInstancia = nullptr;

GerenciadorGrafico::GerenciadorGrafico() : 
    window(sf::VideoMode({1920u, 1080u}), "VALEN")
{
}

GerenciadorGrafico::~GerenciadorGrafico() 
{
}

GerenciadorGrafico* GerenciadorGrafico::getInstance()
{
    if (pInstancia == nullptr)
    {
        pInstancia = new GerenciadorGrafico();
    }
    return pInstancia;
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