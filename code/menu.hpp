#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>
#include "GerenciadorGrafico.hpp"

class Menu {
public:
    Menu();
    ~Menu() = default;

    int executar();

private:
    void set_values();
    void draw_all();
    
    GerenciadorGrafico* pGG;

    sf::Font font;
    sf::Texture texture;
    std::optional<sf::Sprite> bg;

    std::size_t pos{0};
    bool pressed{false};
    bool theselect{false};

    sf::Vector2i pos_mouse;
    sf::Vector2f mouse_coord;

    std::vector<std::string> options;
    std::vector<sf::Text> texts;
    std::vector<sf::Vector2f> coords;
    std::vector<unsigned int> sizes;
};