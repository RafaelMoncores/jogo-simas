#include "GerenciadorGrafico.hpp"
#include "../Entidades/Ente.hpp"

namespace Gerenciadores{
    GerenciadorGrafico* GerenciadorGrafico::pInstancia = nullptr;

    GerenciadorGrafico::GerenciadorGrafico() : 
        window(sf::VideoMode({1920u, 1080u}), "VALEN"),
        viewRect({0.f,0.f},{1920.f,1080.f}),
        uiView(viewRect)
    {
        
    }

    GerenciadorGrafico::~GerenciadorGrafico() 
    {
    }

    GerenciadorGrafico* GerenciadorGrafico::getInstance()
    {
        if (pInstancia == nullptr)
        {
            pInstancia = new GerenciadorGrafico();
        }
        return pInstancia;
    }

    bool GerenciadorGrafico::isWindowOpen() const
    {
        return window.isOpen();
    }

    void GerenciadorGrafico::fecharWindow()
    {
        window.close();
    }

    std::optional<sf::Event> GerenciadorGrafico::pollEvent()
    {
        return window.pollEvent();
    }

    void GerenciadorGrafico::limpar(sf::Color cor)
    {
        window.clear(cor);
    }

    void GerenciadorGrafico::desenhar(const sf::Drawable& desenhavel)
    {
        window.draw(desenhavel);
    }

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

    void GerenciadorGrafico::setViewBounds(float left, float top, float width, float height)
    {
        sf::FloatRect areaVisivel({left, top}, {width, height});
        camera = sf::View(areaVisivel);
        window.setView(camera);
    }

    void GerenciadorGrafico::aplicarView()
    {
        window.setView(camera);
    }

    void GerenciadorGrafico::resetarView()
    {
        window.setView(uiView);
    }
}