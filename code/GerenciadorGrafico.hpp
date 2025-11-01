#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

class GerenciadorGrafico
{
private:
    sf::RenderWindow window;

public:
    GerenciadorGrafico();
    ~GerenciadorGrafico();

    bool isWindowOpen() const;
    void fecharWindow();
    
    std::optional<sf::Event> pollEvent();

    void limpar(sf::Color cor = sf::Color::Green);
    void desenhar(const sf::Drawable& desenhavel);
    void exibir();
};