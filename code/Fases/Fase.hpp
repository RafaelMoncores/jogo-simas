#pragma once
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Gerenciadores/GerenciadorColisoes.hpp"
#include "../Entidades/Obstaculos/Rampa.hpp"      
#include "../Entidades/Obstaculos/Obstaculo.hpp"  
#include <list>
#include <SFML/Graphics.hpp> // Para o bgFase

namespace Fases
{
    class Fase
    {
    private:
        Entidades::Personagens::Jogador* jogador1;
        std::list<Entidades::Obstaculos::Obstaculo*> listaObstaculos;
        Gerenciadores::GerenciadorColisoes gerenciadorColisoes;
        
        // Fundo (movido de Jogo para Fase)
        sf::Texture texFase;
        std::optional<sf::Sprite> bgFase;


    public:
        Fase();
        ~Fase();

        void inicializar(); // Cria jogador e plataformas
        void executar(float delta);
        void desenhar();
    };
}