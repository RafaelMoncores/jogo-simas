#include "menu.hpp"
#include <iostream>
#include <filesystem>

Menu::Menu()
: window(sf::VideoMode({1920u, 1080u}), "Menu SFML") // <-- SFML 3 usa VideoMode({w,h})
{
    set_values();
}

void Menu::set_values()
{
    window.setPosition({0, 0});

    pos = 0;
    pressed = theselect = false;

    // Carrega fonte: SFML3 usa openFromFile para fonts (migration guide).
    if (!font.openFromFile("PressStart2P-Regular.ttf")) {
        std::cerr << "Erro: nao foi possivel carregar fonte PressStart2P-Regular.ttf\n";
    }

    // Carrega textura normalmente (loadFromFile ainda existe para textures)
    if (!texture.loadFromFile("menu.png")) {
        std::cerr << "Erro: nao foi possivel carregar imagem menu.png\n";
    } else {
        // sf::Sprite não tem construtor padrão em SFML 3, tem que construir com a textura
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
    // cria cada sf::Text com a font (sf::Text não tem default constructor em SFML3)
    for (std::size_t i = 0; i < options.size(); ++i) {
        texts.emplace_back(font);              // construtor que recebe font
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

    //winclose.setSize({23.f, 26.f});
    //winclose.setPosition({1178.f, 39.f});
    //winclose.setFillColor(sf::Color::Transparent);
}

void Menu::loop_events()
{
    // Processa todos os eventos (SFML 3: pollEvent retorna std::optional<sf::Event>)
    while (const auto optEvent = window.pollEvent()) {
        const auto &event = *optEvent;

        // Fechar janela
        if (event.is<sf::Event::Closed>()) {
            window.close();
            return;
        }

        // Teclas: trate KeyPressed para uma ação por pressionamento
        if (event.is<sf::Event::KeyPressed>()) {
            const auto *kp = event.getIf<sf::Event::KeyPressed>();
            if (!kp) continue;

            switch (kp->code) {
                case sf::Keyboard::Key::S:
                    // desce — limite máximo é texts.size()-1 (última opção)
                    if (pos < texts.size() - 1) {
                        texts[pos].setOutlineThickness(0.f);
                        ++pos;
                        texts[pos].setOutlineThickness(4.f);
                    }
                    break;

                case sf::Keyboard::Key::W:
                    // sobe — note: se seu primeiro item selecionável é o índice 1, mantém 1 como mínimo
                    if (pos > 1) {
                        texts[pos].setOutlineThickness(0.f);
                        --pos;
                        texts[pos].setOutlineThickness(4.f);
                    }
                    break;

                case sf::Keyboard::Key::Enter:
                    theselect = true;
                    if (pos == 5) { // Quit (ajuste se o índice for outro)
                        window.close();
                    }
                    std::cout << options[pos] << '\n';
                    break;

                default:
                    break;
            }
        }

        // Clique do mouse — use o evento MouseButtonPressed (também evita polling)
        if (event.is<sf::Event::MouseButtonPressed>()) {
            const auto *mb = event.getIf<sf::Event::MouseButtonPressed>();
            if (mb && mb->button == sf::Mouse::Button::Left) {
                // posição do clique já vem no evento (relativa à janela)
                sf::Vector2f clickPos = window.mapPixelToCoords(mb->position);
                if (winclose.getGlobalBounds().contains(clickPos)) {
                    window.close();
                    return;
                }
            }
        }

        // (Opcional) atualize mouse_coord para uso fora dos eventos
        if (event.is<sf::Event::MouseMoved>()) {
            const auto *mm = event.getIf<sf::Event::MouseMoved>();
            if (mm) {
                pos_mouse = mm->position;
                mouse_coord = window.mapPixelToCoords(pos_mouse);
            }
        }
    } // fim while events

    // Se você ainda usa verificações em tempo real (por exemplo hover), atualize aqui:
    // pos_mouse = sf::Mouse::getPosition(window);
    // mouse_coord = window.mapPixelToCoords(pos_mouse);
}

void Menu::draw_all()
{
    window.clear();
    if (bg) window.draw(*bg);
    for (auto &t : texts) {
        window.draw(t);
    }
    // window.draw(winclose); // descomente se quiser ver o retângulo do botão
    window.display();
}

void Menu::run_menu()
{
    while (window.isOpen()) {
        loop_events();
        draw_all();
    }
}
