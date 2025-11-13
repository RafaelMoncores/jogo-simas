#pragma once
#include "../Entidades/Ente.hpp"
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Gerenciadores/GerenciadorColisoes.hpp"
#include "../Entidades/Obstaculos/Trampolim.hpp"      
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
    class Fase : public Entidades::Ente
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

            bool faseConcluida;

            void inicializarUI();

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarPlataformas() = 0;
            virtual void PosarInimigos() = 0;
            virtual void PosarObstaculos() = 0;

        public:
            Fase();
            virtual ~Fase();

            void inicializar();
            virtual void executar(float delta) override;
            bool getFaseConcluida() const;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;
            virtual void salvar();
    };
}