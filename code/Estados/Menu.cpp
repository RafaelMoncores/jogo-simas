#include "Menu.hpp"
#include "../Jogo.hpp"
#include <iostream>
#include <SFML/Window/Mouse.hpp>
#include <vector>

namespace Estados
{
    // ... (Menu::Menu, Menu::centralizarTexto, Menu::set_values_principal permanecem iguais) ...
    Menu::Menu() :
        pGG(Gerenciadores::GerenciadorGrafico::getInstance()),
        enterDebounce(false),
        estadoAtualMenu(EstadoMenu::MenuPrincipal),
        nivelSelecionado(1)
    {
        set_values_principal();
    }

    void Menu::centralizarTexto(sf::Text& texto, float x, float y) {
        sf::FloatRect bounds = texto.getLocalBounds();
        texto.setOrigin({
            bounds.position.x + bounds.size.x / 2.0f, 
            bounds.position.y + bounds.size.y / 2.0f
        });
        texto.setPosition({x, y});
    }

    void Menu::set_values_principal()
    {
        pos = (estadoAtualMenu == EstadoMenu::MenuPrincipal && pos == 0) ? 1 : pos;
        
        sf::Vector2u windowSize = pGG->getWindow().getSize();
        float w = static_cast<float>(windowSize.x);
        float h = static_cast<float>(windowSize.y);

        if (!font.openFromFile("font/PressStart2P-Regular.ttf")) {
            std::cerr << "Erro: nao foi possivel carregar fonte PressStart2P-Regular.ttf\n";
        }
        if (!texture.loadFromFile("tileSets/menu.png")) {
             std::cerr << "Erro: nao foi possivel carregar imagem menu.png\n";
        } else {
            bg.emplace(texture);
            sf::Vector2u texSize = texture.getSize();
            bg->setScale({w / texSize.x, h / texSize.y});
        }

        options = {"Valen", "Play", "Load", "Levels", "Ranking", "Quit"};
        
        unsigned int fontSizeTitle = static_cast<unsigned int>(h * 0.06f); 
        unsigned int fontSizeItem = static_cast<unsigned int>(h * 0.022f); 
        
        sizes = {fontSizeTitle, fontSizeItem, fontSizeItem, fontSizeItem, fontSizeItem, fontSizeItem};

        std::vector<float> y_ratios = {
            0.14f, // Título (Placa Grande)
            0.35f, // 1ª Placa Pequena
            0.48f, // 2ª Placa Pequena
            0.61f, // 3ª Placa Pequena
            0.74f, // 4ª Placa Pequena
            0.87f  // 5ª Placa Pequena (Quit)
        };

        texts.clear();
        texts.reserve(options.size());

        for (std::size_t i = 0; i < options.size(); ++i) {
            texts.emplace_back(font);
            texts[i].setString(options[i]);
            texts[i].setCharacterSize(sizes[i]);
            texts[i].setOutlineColor(sf::Color::Black);
            texts[i].setOutlineThickness(0.f);

            float ratio = (i < y_ratios.size()) ? y_ratios[i] : 0.9f;
            centralizarTexto(texts[i], w / 2.0f, h * ratio);
        }

        if (pos < texts.size()) texts[pos].setOutlineThickness(4.f);
    }

    void Menu::set_values_niveis()
    {
        sf::Vector2u windowSize = pGG->getWindow().getSize();
        float w = static_cast<float>(windowSize.x);
        float h = static_cast<float>(windowSize.y);

        // --- INÍCIO DA MUDANÇA ---
        texts.clear();
        
        // 1. ADICIONA O TÍTULO FIXO (Valen)
        unsigned int fontSizeTitle = static_cast<unsigned int>(h * 0.06f); 
        sf::Text titulo(font, "Valen", fontSizeTitle);
        centralizarTexto(titulo, w / 2.0f, h * 0.14f);
        texts.push_back(titulo);
        // --- FIM DA MUDANÇA ---


        options = {"Level 1", "Level 2", "Back"};
        
        unsigned int fontSizeItem = static_cast<unsigned int>(h * 0.022f);
        // Aumentei o vetor sizes para 4, pois o título (Valen) está no índice 0
        sizes = {fontSizeTitle, fontSizeItem, fontSizeItem, fontSizeItem}; 

        // Usa os mesmos slots das placas pequenas (1, 2 e 3)
        std::vector<float> y_ratios = {
            0.35f, // Posição do "Play"
            0.48f, // Posição do "Load"
            0.61f  // Posição do "Levels"
        };

        // texts.clear() foi movido
        texts.reserve(options.size() + 1); // +1 para o título
        
        for (std::size_t i = 0; i < options.size(); ++i) {
            texts.emplace_back(font);
            texts[i + 1].setString(options[i]); // Pula o índice 0 (título)
            texts[i + 1].setCharacterSize(sizes[i + 1]);
            texts[i + 1].setOutlineColor(sf::Color::Black);
            texts[i + 1].setOutlineThickness(0.f);
            
            float ratio = (i < y_ratios.size()) ? y_ratios[i] : 0.8f;
            centralizarTexto(texts[i + 1], w / 2.0f, h * ratio);
        }

        // Garante que o item selecionável comece no índice 1
        pos = (pos == 0) ? 1 : pos;
        if (pos < texts.size()) texts[pos].setOutlineThickness(4.f);
    }

    void Menu::set_values_ranking(const std::vector<RankingEntry>& ranking1, const std::vector<RankingEntry>& ranking2)
    {
        sf::Vector2u windowSize = pGG->getWindow().getSize();
        float w = static_cast<float>(windowSize.x);
        float h = static_cast<float>(windowSize.y);
        
        if (!texture.loadFromFile("tileSets/menu_ranking.png")) {
            std::cout << "Erro: nao foi possivel carregar imagem menu_ranking.png\n" << std::endl;
        } 
        bg.emplace(texture);
        sf::Vector2u texSize = texture.getSize();
        bg->setScale({w / texSize.x, h / texSize.y});

        texts.clear();
        
        // --- TAMANHOS DE FONTE PROPORCIONAIS ---
        unsigned int sizeTitle = static_cast<unsigned int>(h * 0.06f);
        unsigned int sizeHeader = static_cast<unsigned int>(h * 0.035f);
        // Fonte menor para as linhas da lista, para caberem
        unsigned int sizeText = static_cast<unsigned int>(h * 0.022f); 

        // 1. TÍTULO FIXO NO SEU VALOR (0.14f)
        sf::Text titulo(font, "Ranking", sizeTitle); // Texto de Ranking
        centralizarTexto(titulo, w / 2.0f, h * 0.14f); 
        texts.push_back(titulo);

        // --- POSICIONAMENTO DAS COLUNAS E CONTEÚDO ---
        
        float xCol1 = w * 0.25f; // Posição X da primeira coluna (25%)
        float xCol2 = w * 0.75f; // Posição X da segunda coluna (75%)
        
        // Cabeçalhos (Fase 1/Fase 2)
        float yHeader = h * 0.25f; // Um pouco abaixo do título (25% da tela)
        
        // Início da Lista
        float yListStart = h * 0.32f; // Início do conteúdo da lista
        float lineGap = h * 0.05f; // Espaçamento entre as linhas (5% da tela)

        // FASE 1
        sf::Text hF1(font, "Fase 1", sizeHeader);
        centralizarTexto(hF1, xCol1, yHeader);
        texts.push_back(hF1);

        int i = 0;
        for (const auto& entry : ranking1)
        {
            i++;
            std::string line = std::to_string(i) + ". " + entry.nome + " - " + std::to_string(entry.pontuacao);
            sf::Text t(font, line, sizeText);
            centralizarTexto(t, xCol1, yListStart + ((i-1) * lineGap));
            texts.push_back(t);
        }

        // FASE 2
        sf::Text hF2(font, "Fase 2", sizeHeader);
        centralizarTexto(hF2, xCol2, yHeader);
        texts.push_back(hF2);

        i = 0;
        for (const auto& entry : ranking2)
        {
            i++;
            std::string line = std::to_string(i) + ". " + entry.nome + " - " + std::to_string(entry.pontuacao);
            sf::Text t(font, line, sizeText);
            centralizarTexto(t, xCol2, yListStart + ((i-1) * lineGap));
            texts.push_back(t);
        }

        // 2. BOTÃO VOLTAR NO SLOT DO QUIT (0.87f)
        sf::Text voltar(font, "Voltar", sizeHeader); 
        centralizarTexto(voltar, w / 2.0f, h * 0.87f); 
        texts.push_back(voltar);

        if (pos >= texts.size()) pos = texts.size() - 1;
        texts[pos].setOutlineThickness(4.f);
    }

    void Menu::set_values_modo_jogo()
    {
        sf::Vector2u windowSize = pGG->getWindow().getSize();
        float w = static_cast<float>(windowSize.x);
        float h = static_cast<float>(windowSize.y);

        // --- INÍCIO DA MUDANÇA ---
        texts.clear();
        
        // 1. ADICIONA O TÍTULO FIXO (Valen)
        unsigned int fontSizeTitle = static_cast<unsigned int>(h * 0.06f); 
        sf::Text titulo(font, "Valen", fontSizeTitle);
        centralizarTexto(titulo, w / 2.0f, h * 0.14f);
        texts.push_back(titulo);
        // --- FIM DA MUDANÇA ---

        options = {"1 player", "2 players", "Voltar"};
        
        // Aumentei sizes para 4, o índice 0 é o título
        unsigned int fontSizePlayer = static_cast<unsigned int>(h * 0.018f); 
        unsigned int fontSizeItem = static_cast<unsigned int>(h * 0.022f);
        sizes = {fontSizeTitle, fontSizePlayer, fontSizePlayer, fontSizeItem}; // Título no índice 0

        // Usa os mesmos slots das placas pequenas (1, 2 e 3)
        std::vector<float> y_ratios = {
            0.35f, 
            0.48f, 
            0.61f  
        };

        texts.reserve(options.size() + 1); // +1 para o título
        
        for (std::size_t i = 0; i < options.size(); ++i) {
            texts.emplace_back(font);
            texts[i + 1].setString(options[i]); // Pula o índice 0 (título)
            texts[i + 1].setCharacterSize(sizes[i + 1]);
            texts[i + 1].setOutlineColor(sf::Color::Black);
            texts[i + 1].setOutlineThickness(0.f);
            
            float ratio = (i < y_ratios.size()) ? y_ratios[i] : 0.8f;
            centralizarTexto(texts[i + 1], w / 2.0f, h * ratio);
        }

        // Garante que o item selecionável comece no índice 1
        pos = (pos == 0) ? 1 : pos;
        if (pos < texts.size()) texts[pos].setOutlineThickness(4.f);
    }

    int Menu::executar(const std::vector<RankingEntry>& ranking1, const std::vector<RankingEntry>& ranking2)
    {
        sf::Vector2u windowSize = pGG->getWindow().getSize();
        static sf::Vector2u tamanhoAntigo = windowSize; 

        if (windowSize != tamanhoAntigo)
        {
            tamanhoAntigo = windowSize;
            switch (estadoAtualMenu) {
                case EstadoMenu::MenuPrincipal: set_values_principal(); break;
                case EstadoMenu::MenuNiveis:    set_values_niveis(); break;
                case EstadoMenu::MenuRanking:   set_values_ranking(ranking1, ranking2); break;
                case EstadoMenu::MenuModoJogo:  set_values_modo_jogo(); break;
            }
        }

        pos_mouse = sf::Mouse::getPosition(pGG->getWindow());
        mouse_coord = pGG->mapPixelToCoords(pos_mouse);

        while (const auto optEvent = pGG->pollEvent()) {
            const auto &event = *optEvent;

            if (event.is<sf::Event::Closed>()) {
                return 0;
            }

            if (estadoAtualMenu != EstadoMenu::MenuRanking)
            {
                if (event.is<sf::Event::MouseMoved>()) {
                    for (std::size_t i = 0; i < texts.size(); ++i) {
                        // O título (índice 0) é fixo, ignoramos no mouseover
                        if (i == 0) continue; 
                        
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

                // O mínimo selecionável é o índice 1 (o título é o 0)
                std::size_t min_pos = 1;

                switch (kp->code) {
                    case sf::Keyboard::Key::S:
                    case sf::Keyboard::Key::Down:
                        if (estadoAtualMenu != EstadoMenu::MenuRanking)
                        {
                            if (pos < texts.size() - 1) {
                                texts[pos].setOutlineThickness(0.f);
                                ++pos;
                                texts[pos].setOutlineThickness(4.f);
                            }
                        }
                        break;
                    
                    case sf::Keyboard::Key::W:
                    case sf::Keyboard::Key::Up:
                        if (estadoAtualMenu != EstadoMenu::MenuRanking)
                        {
                            if (pos > min_pos) {
                                texts[pos].setOutlineThickness(0.f);
                                --pos;
                                texts[pos].setOutlineThickness(4.f);
                            }
                        }
                        break;

                    case sf::Keyboard::Key::Enter:
                        if (!enterDebounce) {
                            theselect = true;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (event.is<sf::Event::KeyReleased>()) {
                const auto *kr = event.getIf<sf::Event::KeyReleased>();
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
                        case 1: // Play
                            nivelSelecionado = 1; 
                            estadoAtualMenu = EstadoMenu::MenuModoJogo;
                            set_values_modo_jogo();
                            return -1;
                        case 2: // Load
                            return -1;
                        case 3: // Levels
                            estadoAtualMenu = EstadoMenu::MenuNiveis;
                            set_values_niveis();
                            return -1;
                        case 4: // Ranking
                            estadoAtualMenu = EstadoMenu::MenuRanking;
                            set_values_ranking(ranking1, ranking2); 
                            return -1;
                        case 5: // Quit
                            return 0;
                    }
                    break;
                
                case EstadoMenu::MenuNiveis:
                    switch (pos) {
                        case 1: // Level 1 (Corrigido para índice 1)
                            nivelSelecionado = 1;
                            estadoAtualMenu = EstadoMenu::MenuModoJogo;
                            set_values_modo_jogo();
                            return -1;
                        case 2: // Level 2
                            nivelSelecionado = 2;
                            estadoAtualMenu = EstadoMenu::MenuModoJogo;
                            set_values_modo_jogo();
                            return -1;
                        case 3: // Back
                            estadoAtualMenu = EstadoMenu::MenuPrincipal;
                            set_values_principal();
                            return -1;
                    }
                    break;

                case EstadoMenu::MenuModoJogo:
                    switch (pos) {
                        case 1: // 1 Jogador (Corrigido para índice 1)
                            if (nivelSelecionado == 1) return 10;
                            if (nivelSelecionado == 2) return 20;
                            break;
                        case 2: // 2 Jogadores
                            if (nivelSelecionado == 1) return 11;
                            if (nivelSelecionado == 2) return 21;
                            break;
                        case 3: // Voltar
                            estadoAtualMenu = EstadoMenu::MenuNiveis;
                            set_values_niveis();
                            return -1;
                    }
                    break;

                case EstadoMenu::MenuRanking:
                    estadoAtualMenu = EstadoMenu::MenuPrincipal;
                    set_values_principal();
                    return -1;
            }
        }

        pGG->resetarView();
        if (bg) pGG->desenhar(*bg);
        
        for (std::size_t i = 0; i < texts.size(); ++i) {
            if (i == pos && estadoAtualMenu != EstadoMenu::MenuRanking) {
                // Se for o item selecionado e não for o título (índice 0)
                if (i == 0) {
                    texts[i].setOutlineThickness(0.f);
                } else {
                    texts[i].setOutlineThickness(4.f);
                }
            } else if (i == 0) {
                // O título (índice 0) é fixo, nunca tem contorno de seleção
                 texts[i].setOutlineThickness(0.f);
            } else if (estadoAtualMenu == EstadoMenu::MenuRanking && i == texts.size() - 1) {
                // No Ranking, só o botão Voltar tem contorno
                texts[i].setOutlineThickness((i == pos) ? 4.f : 0.f);
            } else {
                texts[i].setOutlineThickness(0.f);
            }
            
            texts[i].setFillColor(sf::Color::White);
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