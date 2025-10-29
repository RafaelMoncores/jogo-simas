#include <SFML/Graphics.hpp>
#include <optional> 
#include <iostream> // Para std::cerr (nosso log de erro)
#include <stdlib.h> // Para exit() (nosso sistema de "crashar" se der erro)

int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 1400}), "Meu Jogo SFML!");

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("player.png"))
    {
        std::cerr << "ERRO FATAL: Nao foi possivel carregar 'player.png'" << std::endl;
        std::cerr << "Certifique-se que o arquivo esta na mesma pasta do 'main.cpp'." << std::endl;
        exit(1);
    }

    sf::Sprite playerSprite(playerTexture);

    playerSprite.setPosition(sf::Vector2f(400.f, 300.f)); 

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Green);

        window.draw(playerSprite);

        window.display();
    }

    return 0;
}