#pragma once
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Gerenciadores/GerenciadorColisoes.hpp"
#include "../Entidades/Obstaculos/Rampa.hpp"      
#include "../Entidades/Obstaculos/Obstaculo.hpp"  
#include "../Entidades/Personagens/Inimigo.hpp"
//#include <list>
#include "../Listas/ListaObstaculos.hpp"
#include "../Listas/ListaInimigos.hpp"
#include "../Listas/ListaEntidades.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

namespace Fases
{
    class Fase
    {
        private:
            Entidades::Personagens::Jogador* jogador1;

            //std::list<Entidades::Obstaculos::Obstaculo*> listaObstaculos;
            //std::list<Entidades::Personagens::Inimigo*> listaInimigos;

            Listas::ListaObstaculos listaObstaculos;
            Listas::ListaInimigos listaInimigos;
            Listas::ListaEntidades listaEntidades;

            Gerenciadores::GerenciadorColisoes gerenciadorColisoes;
            
            // Fundo (movido de Jogo para Fase)
            sf::Texture texFase;
            std::optional<sf::Sprite> bgFase;

            sf::Font uiFont;
            std::optional<sf::Text> vidasText;
            void inicializarUI();

        public:
            Fase();
            ~Fase();

            void inicializar();
            void executar(float delta);
            void desenhar();
    };
}