#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>

class Menu {
public:
    Menu();
    ~Menu() = default;

    void run_menu();

private:
    void set_values();
    void loop_events();
    void draw_all();

    sf::RenderWindow window;
    sf::RectangleShape winclose;

    sf::Font font;
    sf::Texture texture;                 // textura carregada
    std::optional<sf::Sprite> bg;        // sprite construído somente depois que a texture é carregada

    std::size_t pos{0};
    bool pressed{false};
    bool theselect{false};

    sf::Vector2i pos_mouse;
    sf::Vector2f mouse_coord;

    std::vector<std::string> options;
    std::vector<sf::Text> texts;         // construímos cada sf::Text com a font após carregar a font
    std::vector<sf::Vector2f> coords;
    std::vector<unsigned int> sizes;
};
