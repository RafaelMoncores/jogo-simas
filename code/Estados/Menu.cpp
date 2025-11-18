#include "Menu.hpp"
#include "../Jogo.hpp"
#include <iostream>
#include <SFML/Window/Mouse.hpp>

namespace Estados
{
    Menu::Menu() :
        pGG(Gerenciadores::GerenciadorGrafico::getInstance()),
        enterDebounce(false),
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

    void Menu::set_values_ranking(const std::vector<RankingEntry>& ranking1, const std::vector<RankingEntry>& ranking2)
    {
        pressed = theselect = false;
        
        // --- ALTERAÇÃO 1: MUDAR O BACKGROUND ---
        // (Assumindo que você tem uma imagem "ranking.png" na sua pasta)
        if (!texture.loadFromFile("tileSets/menu_ranking.png")) {
            std::cerr << "Erro: nao foi possivel carregar imagem ranking.png\n";
            // (Opcional: voltar para o menu principal se falhar)
        } else {
            bg.emplace(texture);
        }
        // ----------------------------------------

        texts.clear();
        options.clear();
        coords.clear();
        sizes.clear();

        // --- ALTERAÇÃO 3: LAYOUT DO RANKING DIVIDIDO ---
        
        // Título Principal
        options.push_back("Ranking");
        coords.push_back({1920.f / 2.f, 100.f});
        sizes.push_back(70);

        // --- Coluna da Fase 1 (Esquerda) ---
        options.push_back("Fase 1");
        coords.push_back({(1920.f / 4.f), 250.f}); // 1/4 da tela
        sizes.push_back(50);
        
        options.push_back("Pos"); options.push_back("Nome"); options.push_back("Pontos");
        coords.push_back({150.f, 350.f}); coords.push_back({350.f, 350.f}); coords.push_back({700.f, 350.f});
        sizes.push_back(30); sizes.push_back(30); sizes.push_back(30);

        float y_pos = 420.f;
        int i = 0;
        for (const auto& entry : ranking1)
        {
            i++;
            options.push_back(std::to_string(i) + "."); // Pos
            options.push_back(entry.nome);              // Nome
            options.push_back(std::to_string(entry.pontuacao)); // Pontos

            coords.push_back({150.f, y_pos});
            coords.push_back({350.f, y_pos});
            coords.push_back({700.f, y_pos});
            sizes.push_back(25); sizes.push_back(25); sizes.push_back(25);
            y_pos += 50.f;
        }

        // --- Coluna da Fase 2 (Direita) ---
        options.push_back("Fase 2");
        coords.push_back({(1920.f / 4.f) * 3.f, 250.f}); // 3/4 da tela
        sizes.push_back(50);

        options.push_back("Pos"); options.push_back("Nome"); options.push_back("Pontos");
        coords.push_back({1150.f, 350.f}); coords.push_back({1350.f, 350.f}); coords.push_back({1700.f, 350.f});
        sizes.push_back(30); sizes.push_back(30); sizes.push_back(30);

        y_pos = 420.f;
        i = 0;
        for (const auto& entry : ranking2)
        {
            i++;
            options.push_back(std::to_string(i) + ".");
            options.push_back(entry.nome);
            options.push_back(std::to_string(entry.pontuacao));

            coords.push_back({1150.f, y_pos});
            coords.push_back({1350.f, y_pos});
            coords.push_back({1700.f, y_pos});
            sizes.push_back(25); sizes.push_back(25); sizes.push_back(25);
            y_pos += 50.f;
        }

        // --- Botão Voltar (O único selecionável) ---
        options.push_back("Voltar");
        coords.push_back({1920.f / 2.f, 950.f});
        sizes.push_back(50);

        // Cria os objetos sf::Text
        for (std::size_t i = 0; i < options.size(); ++i) {
            texts.emplace_back(font);
            texts[i].setString(options[i]);
            texts[i].setCharacterSize(sizes[i]);
            
            sf::FloatRect bounds = texts[i].getLocalBounds();
            texts[i].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
            texts[i].setPosition(coords[i]);
        }

        // --- ALTERAÇÃO 2: SELECIONAR APENAS "VOLTAR" ---
        // Força a posição 'pos' a ser o último item (o botão "Voltar")
        pos = texts.size() - 1;
        // ---------------------------------------------
    }

    int Menu::executar(const std::vector<RankingEntry>& ranking1, const std::vector<RankingEntry>& ranking2)
    {
        pos_mouse = sf::Mouse::getPosition(pGG->getWindow());
        mouse_coord = pGG->mapPixelToCoords(pos_mouse);

        // O seu 'while' loop (está correto)
        while (const auto optEvent = pGG->pollEvent()) {
            const auto &event = *optEvent; // 'event' é uma REFERÊNCIA (usa '.')

            if (event.is<sf::Event::Closed>()) {
                return 0;
            }

            // --- CORREÇÃO 1: REMOVA o bloco 'if (event->is<...>' que eu sugeri ---
            // (O bloco que causou os erros foi removido)


            // --- CORREÇÃO 2: Embrulhe o seu 'MouseMoved' ---
            if (estadoAtualMenu != EstadoMenu::MenuRanking)
            {
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
            }

            // O seu 'MouseButtonPressed' está correto, não mexe
            if (event.is<sf::Event::MouseButtonPressed>()) {
                if (event.getIf<sf::Event::MouseButtonPressed>()->button == sf::Mouse::Button::Left) {
                    if (texts[pos].getGlobalBounds().contains(mouse_coord)) {
                        theselect = true;
                    }
                }
            }

            // --- CORREÇÃO 3: Altere o seu 'KeyPressed' ---
            if (event.is<sf::Event::KeyPressed>()) {
                const auto *kp = event.getIf<sf::Event::KeyPressed>();
                if (!kp) continue;

                std::size_t min_pos = (estadoAtualMenu == EstadoMenu::MenuPrincipal) ? 1 : 0;

                switch (kp->code) {
                    // Adicione a verificação de estado AQUI
                    case sf::Keyboard::Key::S:
                        if (estadoAtualMenu != EstadoMenu::MenuRanking) // <-- ADICIONAR
                        {
                            if (pos < texts.size() - 1) {
                                texts[pos].setOutlineThickness(0.f);
                                ++pos;
                                texts[pos].setOutlineThickness(4.f);
                            }
                        }
                        break;
                    
                    // Adicione a verificação de estado AQUI
                    case sf::Keyboard::Key::W:
                        if (estadoAtualMenu != EstadoMenu::MenuRanking) // <-- ADICIONAR
                        {
                            if (pos > min_pos) {
                                texts[pos].setOutlineThickness(0.f);
                                --pos;
                                texts[pos].setOutlineThickness(4.f);
                            }
                        }
                        break;

                    // 'Enter' deve funcionar em todos os estados
                    case sf::Keyboard::Key::Enter:
                        if (!enterDebounce) { // Só seleciona se não estiver em debounce
                            theselect = true;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (event.is<sf::Event::KeyReleased>()) {
                const auto *kr = event.getIf<sf::Event::KeyReleased>();
                // Se o usuário SOLTOU o Enter, paramos o debounce
                if (kr && kr->code == sf::Keyboard::Key::Enter) {
                    enterDebounce = false; 
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
                        case 4: 
                            estadoAtualMenu = EstadoMenu::MenuRanking;
                            set_values_ranking(ranking1, ranking2); 
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
                    // Como 'pos' está travado no botão "Voltar",
                    // qualquer 'Enter' aqui significa "Voltar".
                    estadoAtualMenu = EstadoMenu::MenuPrincipal;
                    set_values_principal();
                    return -1;
            }
        }

        pGG->resetarView();
        if (bg) pGG->desenhar(*bg);
        
        for (std::size_t i = 0; i < texts.size(); ++i) {
            if (i == pos) {
                texts[i].setOutlineThickness(4.f);
                texts[i].setFillColor(sf::Color::White);
            } else {
                texts[i].setOutlineThickness(0.f);
                texts[i].setFillColor(sf::Color::White);
            }
            pGG->desenhar(texts[i]);
        }
        
        return -1;
    }

    void Menu::resetInput()
    {
        theselect = false;
        pressed = false;
        enterDebounce = true;
    }
}