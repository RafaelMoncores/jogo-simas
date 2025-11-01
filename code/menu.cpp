#include "Menu.hpp"
#include <iostream>

Menu::Menu() :
    pGG(GerenciadorGrafico::getInstance()) 
{
    set_values();
}

void Menu::set_values()
{

    pos = 0;
    pressed = theselect = false;

    if (!font.openFromFile("PressStart2P-Regular.ttf")) {
        std::cerr << "Erro: nao foi possivel carregar fonte PressStart2P-Regular.ttf\n";
    }

    if (!texture.loadFromFile("menu.png")) {
        std::cerr << "Erro: nao foi possivel carregar imagem menu.png\n";
    } else {
        bg.emplace(texture);
    }

    pos_mouse = {0, 0};
    mouse_coord = {0.f, 0.f};

    options = {"Valen", "Play", "Load", "Levels", "Ranking", "Quit"};
    coords = {
        {831.f, 128.f},
        {930.f, 370.f},
        {930.f, 508.f},
        {910.f, 648.f},
        {900.f, 789.f},
        {930.f, 930.f}
    };
    sizes = {51, 18, 18, 18, 18, 18};

    texts.clear();
    texts.reserve(options.size());
    for (std::size_t i = 0; i < options.size(); ++i) {
        texts.emplace_back(font);
        texts[i].setString(options[i]);
        texts[i].setCharacterSize(sizes[i]);
        texts[i].setOutlineColor(sf::Color::Black);
        texts[i].setOutlineThickness(0.f);
        texts[i].setPosition(coords[i]);
    }

    if (texts.size() > 1) {
        texts[1].setOutlineThickness(4.f);
        pos = 1;
    }
}

int Menu::executar()
{
    while (const auto optEvent = pGG->pollEvent()) {
        const auto &event = *optEvent;

        if (event.is<sf::Event::Closed>()) {
            return 0;
        }

        if (event.is<sf::Event::KeyPressed>()) {
            const auto *kp = event.getIf<sf::Event::KeyPressed>();
            if (!kp) continue;

            switch (kp->code) {
                case sf::Keyboard::Key::S:
                    if (pos < texts.size() - 1) {
                        texts[pos].setOutlineThickness(0.f);
                        ++pos;
                        texts[pos].setOutlineThickness(4.f);
                    }
                    break;
                case sf::Keyboard::Key::W:
                    if (pos > 1) {
                        texts[pos].setOutlineThickness(0.f);
                        --pos;
                        texts[pos].setOutlineThickness(4.f);
                    }
                    break;
                case sf::Keyboard::Key::Enter:
                    theselect = true;
                    std::cout << options[pos] << '\n';
                    
                    if (pos == 5) {
                        return 0;
                    }
                    if (pos == 1) {
                        return 1;
                    }
                    break;
                default:
                    break;
            }
        }
    }

    if (bg) pGG->desenhar(*bg);
    for (auto &t : texts) {
        pGG->desenhar(t);
    }
    
    return -1;
}
