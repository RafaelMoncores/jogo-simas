#include "Menu.hpp"
#include <iostream>
#include <SFML/Window/Mouse.hpp>

namespace Estados
{
    Menu::Menu() :
        pGG(Gerenciadores::GerenciadorGrafico::getInstance()),
        estadoAtualMenu(EstadoMenu::MenuPrincipal)
    {
        set_values_principal();
    }

    void Menu::set_values_principal()
    {
        pos = 0;
        pressed = theselect = false;

        if (!font.openFromFile("font/PressStart2P-Regular.ttf")) {
            std::cerr << "Erro: nao foi possivel carregar fonte PressStart2P-Regular.ttf\n";
        }
        if (!texture.loadFromFile("tileSets/menu.png")) {
             std::cerr << "Erro: nao foi possivel carregar imagem menu.png\n";
        } else {
            bg.emplace(texture);
        }

        pos_mouse = {0, 0};
        mouse_coord = {0.f, 0.f};

        // Opções do menu principal
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

    void Menu::set_values_niveis()
    {
        pos = 0;
        pressed = theselect = false;

        pos_mouse = {0, 0};
        mouse_coord = {0.f, 0.f};

        options = {"Level 1", "Level 2", "Back"};
        coords = {
            {900.f, 370.f},
            {900.f, 508.f},
            {930.f, 648.f}
        };
        sizes = {18, 18, 18};

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

        if (!texts.empty()) {
            texts[0].setOutlineThickness(4.f);
            pos = 0;
        }
    }

    int Menu::executar()
    {
        pos_mouse = sf::Mouse::getPosition(pGG->getWindow());
        mouse_coord = pGG->mapPixelToCoords(pos_mouse);

        while (const auto optEvent = pGG->pollEvent()) {
            const auto &event = *optEvent;

            if (event.is<sf::Event::Closed>()) {
                return 0;
            }

            if (event.is<sf::Event::MouseMoved>()) {
                for (std::size_t i = 0; i < texts.size(); ++i) {
                    if (estadoAtualMenu == EstadoMenu::MenuPrincipal && i == 0) continue; 
                    
                    if (texts[i].getGlobalBounds().contains(mouse_coord)) {
                        if (pos != i) {
                            texts[pos].setOutlineThickness(0.f);
                            pos = i;
                            texts[pos].setOutlineThickness(4.f);
                        }
                        break;
                    }
                }
            }

            if (event.is<sf::Event::MouseButtonPressed>()) {
                if (event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                    if (texts[pos].getGlobalBounds().contains(mouse_coord)) {
                        theselect = true;
                    }
                }
            }

            if (event.is<sf::Event::KeyPressed>()) {
                const auto *kp = event.getIf<sf::Event::KeyPressed>();
                if (!kp) continue;

                std::size_t min_pos = (estadoAtualMenu == EstadoMenu::MenuPrincipal) ? 1 : 0;

                switch (kp->code) {
                    case sf::Keyboard::Key::S:
                        if (pos < texts.size() - 1) {
                            texts[pos].setOutlineThickness(0.f);
                            ++pos;
                            texts[pos].setOutlineThickness(4.f);
                        }
                        break;
                    case sf::Keyboard::Key::W:
                        if (pos > min_pos) {
                            texts[pos].setOutlineThickness(0.f);
                            --pos;
                            texts[pos].setOutlineThickness(4.f);
                        }
                        break;
                    case sf::Keyboard::Key::Enter:
                        theselect = true;
                        break;
                    default:
                        break;
                }
            }
        }

        if (theselect) {
            theselect = false;

            switch (estadoAtualMenu)
            {
                case EstadoMenu::MenuPrincipal:
                    switch (pos) {
                        case 1:
                            return 1;
                        case 3:
                            estadoAtualMenu = EstadoMenu::MenuNiveis;
                            set_values_niveis();
                            return -1;
                        case 5:
                            return 0;
                    }
                    break;
                
                case EstadoMenu::MenuNiveis:
                    switch (pos) {
                        case 0:
                            return 1;
                        case 1:
                            return 2;
                        case 2:
                            estadoAtualMenu = EstadoMenu::MenuPrincipal;
                            set_values_principal();
                            return -1;
                    }
                    break;
            }
        }

        pGG->resetarView();
        if (bg) pGG->desenhar(*bg);
        for (auto &t : texts) {
            pGG->desenhar(t);
        }
        
        return -1;
    }
}