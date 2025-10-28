#include <SFML/Graphics.hpp>
#include <optional> // Necessário para o novo pollEvent

int main()
{
    // Mudança 1: VideoMode agora espera um sf::Vector2u
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Meu Jogo SFML!");

    // Loop principal do jogo
    while (window.isOpen())
    {
        // Mudança 2: pollEvent() não recebe mais um argumento.
        // Ele retorna um std::optional<sf::Event>
        while (auto event = window.pollEvent())
        {
            // Mudança 3: Precisamos checar se o evento existe e usar ->
            // Mudança 4: O 'type' foi substituído por 'is<T>()'
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Limpa a tela com a cor verde
        window.clear(sf::Color::Green);

        // Exibe o que foi desenhado
        window.display();
    }

    return 0;
}