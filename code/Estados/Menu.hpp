#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Gerenciadores/GerenciadorEventos.hpp"

struct RankingEntry;

namespace Estados
{
    class Menu : public OuvinteEventos {
        public:
            Menu();
            ~Menu() = default;

            int executar(const std::vector<RankingEntry>& ranking1, const std::vector<RankingEntry>& ranking2);
            void resetInput();
            virtual void tratarEvento(const sf::Event& evento) override;

        private:
            enum class EstadoMenu { MenuPrincipal, MenuNiveis, MenuRanking, MenuModoJogo };
            EstadoMenu estadoAtualMenu;
            
            void centralizarTexto(sf::Text& texto, float x, float y);
            void set_values_principal();
            void set_values_niveis();
            void set_values_ranking(const std::vector<RankingEntry>& ranking1, const std::vector<RankingEntry>& ranking2);
            void set_values_modo_jogo();
            int nivelSelecionado;
            
            Gerenciadores::GerenciadorGrafico* pGG;

            sf::Font font;
            sf::Texture texture;
            std::optional<sf::Sprite> bg;

            std::size_t pos{0};
            bool pressed{false};
            bool theselect{false};
            bool enterDebounce{false};

            sf::Vector2i pos_mouse;
            sf::Vector2f mouse_coord;

            std::vector<std::string> options;
            std::vector<sf::Text> texts;
            std::vector<sf::Vector2f> coords;
            std::vector<unsigned int> sizes;
    };
}