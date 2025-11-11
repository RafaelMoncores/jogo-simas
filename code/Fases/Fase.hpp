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
        protected: 
            Entidades::Personagens::Jogador* jogador1;

            Listas::ListaObstaculos listaObstaculos;
            Listas::ListaInimigos listaInimigos;
            Listas::ListaEntidades listaEntidades;

            Gerenciadores::GerenciadorColisoes gerenciadorColisoes;
            
            sf::Texture texFase;
            std::optional<sf::Sprite> bgFase;

            sf::Font uiFont;
            std::optional<sf::Text> vidasText;

            void inicializarUI();

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;

        public:
            Fase();
            virtual ~Fase();

            void inicializar();
            void executar(float delta);
            void desenhar();
    };
}