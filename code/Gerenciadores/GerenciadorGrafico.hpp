#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

namespace Gerenciadores{
    class GerenciadorGrafico
    {
        private:
            // Instância estática para o Singleton
            static GerenciadorGrafico* pInstancia;
            sf::RenderWindow window;

            sf::View camera;

            // Construtor privado para impedir instanciação externa
            GerenciadorGrafico();

        public:
            ~GerenciadorGrafico();

            // Método de acesso à instância
            static GerenciadorGrafico* getInstance();

            // Prevenção de cópia
            GerenciadorGrafico(const GerenciadorGrafico&) = delete;
            GerenciadorGrafico& operator=(const GerenciadorGrafico&) = delete;

            // Métodos de controle da janela
            bool isWindowOpen() const;
            void fecharWindow();
            std::optional<sf::Event> pollEvent();

            // Métodos de renderização
            void limpar(sf::Color cor = sf::Color::Black);
            void desenhar(const sf::Drawable& desenhavel);
            void exibir();

            // Configura a câmera para mostrar uma região específica do mundo
            void setViewBounds(float left, float top, float width, float height);
            void aplicarView();
            // Reseta a câmera para a padrão (para o Menu/UI)
            void resetarView();
    };
}