#include "GerenciadorGrafico.hpp"
#include "../Entidades/Ente.hpp"

namespace Gerenciadores{
    // Inicialização do ponteiro estático (necessário para o Linker em C++)
    GerenciadorGrafico* GerenciadorGrafico::pInstancia = nullptr;

    // Construtor: Cria a janela usando inicialização uniforme (C++11)
    // A resolução 1920x1080 é definida hardcoded aqui
    GerenciadorGrafico::GerenciadorGrafico() : 
        window(sf::VideoMode({1920u, 1080u}), "VALEN")
    {
    }

    GerenciadorGrafico::~GerenciadorGrafico() 
    {
        // O sf::RenderWindow é destruído automaticamente pelo RAII
    }

    // Padrão Singleton: Garante que só exista uma janela no jogo inteiro
    GerenciadorGrafico* GerenciadorGrafico::getInstance()
    {
        if (pInstancia == nullptr)
        {
            pInstancia = new GerenciadorGrafico();
        }
        return pInstancia;
    }

    // Wrappers simples para encapsular métodos da janela
    bool GerenciadorGrafico::isWindowOpen() const
    {
        return window.isOpen(); //retorna true or false
    }

    void GerenciadorGrafico::fecharWindow()
    {
        window.close();
    }

    // SFML 3.0+: pollEvent agora retorna std::optional
    // Isso evita o padrão antigo de passar evento por referência
    std::optional<sf::Event> GerenciadorGrafico::pollEvent()
    {
        return window.pollEvent();
    }

    sf::RenderWindow& GerenciadorGrafico::getWindow()
    {
        return window;
    }

    // Converte pixel da tela (Mouse) para coordenada do mundo (Jogo)
    // Essencial para cliques corretos quando a câmera se move
    sf::Vector2f GerenciadorGrafico::mapPixelToCoords(sf::Vector2i pixelPos)
    {
        return window.mapPixelToCoords(pixelPos);
    }

    // --- Métodos de Renderização ---
    
    void GerenciadorGrafico::limpar(sf::Color cor)
    {
        window.clear(cor); //com a janela já criada, limpa e seta uma cor sólida antes de desenhar algo
    }

    void GerenciadorGrafico::desenhar(const sf::Drawable& desenhavel)
    {
        window.draw(desenhavel);
    }

    // Sobrecarga para permitir que Entidades se desenhem
    // Utiliza polimorfismo (método virtual desenhar() na classe Ente)
    void GerenciadorGrafico::desenha(Entidades::Ente* pE)
    {
        if (pE)
        {
            pE->desenhar();
        }
    }

    void GerenciadorGrafico::exibir()
    {
        window.display();
    }

    // --- Controle de Câmera (View) ---

    // Define qual área do mundo o jogador está vendo (Zoom/Scroll)
    void GerenciadorGrafico::setViewBounds(float left, float top, float width, float height)
    {
        sf::FloatRect areaVisivel({left, top}, {width, height});
        camera = sf::View(areaVisivel);
        window.setView(camera);
    }

    // Aplica a câmera configurada (usado no loop de jogo)
    void GerenciadorGrafico::aplicarView()
    {
        window.setView(camera);
    }

    // Reseta para a visão padrão (usado para desenhar GUI/Menu estático)
    void GerenciadorGrafico::resetarView()
    {
        window.setView(window.getDefaultView());
    }
}