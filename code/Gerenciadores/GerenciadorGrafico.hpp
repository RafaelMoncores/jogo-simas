#pragma once

#include <SFML/Graphics.hpp>
#include <optional>

namespace Entidades { 
    class Ente; 
}

namespace Gerenciadores{
    class GerenciadorGrafico
    {
        private:
            // Padrão Singleton: única instância compartilhada por todo o jogo
            static GerenciadorGrafico* pInstancia;
            sf::RenderWindow window;
            sf::View camera;

            // Construtor privado evita que outras classes criem novas janelas
            GerenciadorGrafico();

        public:
            ~GerenciadorGrafico();

            // Ponto de acesso global à instância
            static GerenciadorGrafico* getInstance();

            // Remove construtor de cópia e atribuição para garantir unicidade
            GerenciadorGrafico(const GerenciadorGrafico&) = delete;
            GerenciadorGrafico& operator=(const GerenciadorGrafico&) = delete;

            // Wrappers para controle da janela SFML
            bool isWindowOpen() const;
            void fecharWindow();
            
            // Retorna um optional (novidade SFML 3) para eventos
            std::optional<sf::Event> pollEvent();

            sf::RenderWindow& getWindow();
            sf::Vector2f mapPixelToCoords(sf::Vector2i pixelPos);

            // Wrappers de renderização para desacoplar a lógica do jogo da biblioteca gráfica
            void limpar(sf::Color cor = sf::Color::Black);
            void desenhar(const sf::Drawable& desenhavel);
            void exibir();
            void desenha(Entidades::Ente* pE);

            // Controle de Câmera (View)
            void setViewBounds(float left, float top, float width, float height);
            void aplicarView();
            void resetarView();
    };
}