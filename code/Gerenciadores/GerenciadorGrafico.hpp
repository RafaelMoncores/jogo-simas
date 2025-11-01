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
    };
}