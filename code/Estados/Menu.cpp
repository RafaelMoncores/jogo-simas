#include "Menu.hpp"
#include "../Jogo.hpp"
#include <iostream>
#include <SFML/Window/Mouse.hpp>

namespace Estados
{
    Menu::Menu() :
        pGG(Gerenciadores::GerenciadorGrafico::getInstance()),
        estadoAtualMenu(EstadoMenu::MenuPrincipal) // Começa no Principal
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

    void Menu::set_values_ranking(const std::vector<RankingEntry>& ranking)
    {
        pos = 0;
        pressed = theselect = false;
        
        // (Reutiliza a mesma fonte e fundo do menu principal)

        // Limpa os textos antigos
        texts.clear();
        options.clear();
        coords.clear();
        sizes.clear();

        // 1. Título e Cabeçalhos
        options.push_back("Ranking");
        options.push_back("Pos");
        options.push_back("Nome");
        options.push_back("Pontos");
        
        coords.push_back({1920.f/2.f, 100.f}); // Título
        coords.push_back({300.f, 250.f});      // "Pos"
        coords.push_back({800.f, 250.f});      // "Nome"
        coords.push_back({1400.f, 250.f});     // "Pontos"

        sizes.push_back(70);
        sizes.push_back(40);
        sizes.push_back(40);
        sizes.push_back(40);

        // 2. Adiciona as entradas do ranking
        int i = 0;
        float y_pos = 350.f;
        for (const auto& entry : ranking)
        {
            i++;
            options.push_back(std::to_string(i) + "."); // Pos
            options.push_back(entry.nome);              // Nome
            options.push_back(std::to_string(entry.pontuacao)); // Pontos

            coords.push_back({300.f, y_pos});
            coords.push_back({800.f, y_pos});
            coords.push_back({1400.f, y_pos});

            sizes.push_back(30);
            sizes.push_back(30);
            sizes.push_back(30);

            y_pos += 60.f; // Próxima linha
        }

        // 3. Botão Voltar
        options.push_back("Voltar");
        coords.push_back({1920.f/2.f, 950.f});
        sizes.push_back(50);

        // 4. Cria os objetos sf::Text
        for (std::size_t i = 0; i < options.size(); ++i) {
            texts.emplace_back(font);
            texts[i].setString(options[i]);
            texts[i].setCharacterSize(sizes[i]);
            
            // Centraliza o texto
            sf::FloatRect bounds = texts[i].getLocalBounds();
            texts[i].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
            texts[i].setPosition(coords[i]);
        }
    }

    int Menu::executar(const std::vector<RankingEntry>& ranking)
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
                        case 4: // Ranking
                            estadoAtualMenu = EstadoMenu::MenuRanking;
                            set_values_ranking(ranking); // Passa os dados
                            return -1;
                        case 5: //Quit
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
                case EstadoMenu::MenuRanking:
                    // A única opção selecionável é "Voltar"
                    // (que será a última posição 'pos')
                    if (pos == texts.size() - 1) 
                    {
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
            // Lógica de seleção (highlight)
            if (estadoAtualMenu == EstadoMenu::MenuRanking && texts.size() > 0)
            {
                // No ranking, só o "Voltar" é selecionável
                if (pos == texts.size() - 1)
                    t.setFillColor(sf::Color::Red);
                else
                    t.setFillColor(sf::Color::White);
            }
            // (A sua lógica de highlight do MenuPrincipal/Niveis já está aqui...)
            
            pGG->desenhar(t);
        }
        
        return -1;
    }
}