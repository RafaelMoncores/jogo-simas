#pragma once
#include "../Entidades/Ente.hpp"
#include "../Entidades/Personagens/Jogador.hpp"
#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Gerenciadores/GerenciadorColisoes.hpp"
#include "../Entidades/Obstaculos/Trampolim.hpp"      
#include "../Entidades/Obstaculos/Obstaculo.hpp"  
#include "../Entidades/Personagens/Inimigo.hpp"
#include "../Gerenciadores/GerenciadorEventos.hpp"
#include "../Listas/ListaEntidades.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

class Jogo;

namespace Fases
{
    class Fase : public Entidades::Ente, public OuvinteEventos
    {
        protected: 
            Jogo* pJogo;
            int faseNum;

            Entidades::Personagens::Jogador* jogador1;
            Entidades::Personagens::Jogador* jogador2;
            bool modoDoisJogadores;
            sf::Vector2f posJogador2Inicial;
            sf::Vector2f posJogadorInicial;

            Listas::ListaEntidades listaEntidades;

            // O Gerenciador agora armazena seus próprios vetores/listas STL
            Gerenciadores::GerenciadorColisoes gerenciadorColisoes;
            
            sf::Texture texFase;
            std::optional<sf::Sprite> bgFase;
            sf::Font uiFont;
            std::optional<sf::Text> vidasTextP1;
            std::optional<sf::Text> pontosTextP1;
            std::optional<sf::Text> vidasTextP2;
            std::optional<sf::Text> pontosTextP2;
            std::optional<sf::Text> tentativasText;
            int numTentativas;
            std::optional<sf::Text> pontuacaoFinalText;
            int pontuacaoFinalCache; 
            bool enterPressionado; 
            bool navPressionado;
            enum class EstadoFim { MostrandoOpcoes, PedindoIniciais };
            EstadoFim estadoFim;
            int posBotaoFim; 
            std::string iniciais;
            std::optional<sf::Text> botaoSalvarText;
            std::optional<sf::Text> botaoMenuText;
            std::optional<sf::Text> inputIniciaisText;
            bool faseConcluida;

            void inicializarUI();
            void processarInputIniciais(const sf::Event& evento);

            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarPlataformas() = 0;
            virtual void PosarInimigos() = 0;
            virtual void PosarObstaculos() = 0;

        public:
            Fase(Jogo* pJogo, int num, bool doisJogadores);
            virtual ~Fase();

            void inicializar();
            virtual void executar(float delta) override;
            bool getFaseConcluida() const;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;
            void processarEvento(const sf::Event& evento);
            virtual void salvar();
            virtual void tratarEvento(const sf::Event& evento) override;
    };
}