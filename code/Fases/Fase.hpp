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
#include <map>
#include <string>

class Jogo;

namespace Fases
{
    // Classe base abstrata que gerencia o ciclo de vida de um nível (Update, Render, Colisão)
    // Herda de OuvinteEventos para processar inputs específicos (como digitar nome no ranking)
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

            // Cada fase possui seu próprio gerenciador de física
            Gerenciadores::GerenciadorColisoes gerenciadorColisoes;
            
            // --- Elementos Gráficos e UI ---
            sf::Texture texFase;
            
            // std::optional (C++17): Permite adiar a inicialização de objetos SFML
            // evitando construtores vazios ou alocação dinâmica desnecessária
            std::optional<sf::Sprite> bgFase;
            sf::Font uiFont;
            
            std::optional<sf::Text> vidasTextP1;
            std::optional<sf::Text> pontosTextP1;
            std::optional<sf::Text> vidasTextP2;
            std::optional<sf::Text> pontosTextP2;
            std::optional<sf::Text> tentativasText;
            std::optional<sf::Text> pontuacaoFinalText;
            
            // --- Controle de Estado e Fim de Jogo ---
            int numTentativas;
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
            int jogadorQueFinalizar; 

            void inicializarUI();
            void processarInputIniciais(const sf::Event& evento);

            // Métodos Factory (Template Method Pattern) que as fases filhas devem implementar
            virtual void criarObstaculos() = 0;
            virtual void criarInimigos() = 0;
            virtual void criarPlataformas() = 0;
            virtual void PosarInimigos() = 0;
            virtual void PosarObstaculos() = 0;

        public:
            Fase(Jogo* pJogo, int num, bool doisJogadores);
            Fase() {};
            virtual ~Fase();

            void inicializar();
            // Versão especial de inicialização para Load Game (não cria inimigos padrão)
            void inicializarParaLoad(); 
            
            virtual void executar(float delta) override;
            bool getFaseConcluida() const;
            virtual void desenhar() override;
            virtual sf::FloatRect getBoundingBox() const override;
            
            void processarEvento(const sf::Event& evento);
            virtual void salvar();
            virtual void tratarEvento(const sf::Event& evento) override;
            
            // --- Sistema de Persistência (Snapshots) ---
            virtual void salvarComNome(const std::string& caminho);
            virtual std::string salvarParaString();
            
            // Métodos auxiliares para reconstruir o nível a partir de dados salvos
            void limparEntidadesDinamicas();
            void restaurarPlataforma(const std::map<std::string, std::string>& kv);
            void restaurarTrampolim(const std::map<std::string, std::string>& kv);
            void restaurarBolaDeFogo(const std::map<std::string, std::string>& kv);
            void restaurarVampiro(const std::map<std::string, std::string>& kv);
            void restaurarGosma(const std::map<std::string, std::string>& kv);
            void restaurarDragao(const std::map<std::string, std::string>& kv);
            
            Entidades::Personagens::Jogador* getJogador1() { return jogador1; }
            Entidades::Personagens::Jogador* getJogador2() { return jogador2; }
    };
}