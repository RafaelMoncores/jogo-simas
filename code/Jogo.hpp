#pragma once

#include "Gerenciadores/GerenciadorGrafico.hpp"
#include "Gerenciadores/GerenciadorEventos.hpp"
#include "Entidades/Personagens/Jogador.hpp"
#include "Estados/Menu.hpp"
#include "Fases/Fase.hpp"
#include <SFML/System/Clock.hpp>
#include <vector>     
#include <string>     
#include <algorithm>  
#include <functional>
#include <map> 

// Estrutura auxiliar para ordenação do ranking (C++ Sort)
struct RankingEntry {
    std::string nome;
    int pontuacao;

    // Operador necessário para std::sort organizar decrescente
    bool operator>(const RankingEntry& other) const {
        return pontuacao > other.pontuacao;
    }
};

class Jogo
{
    private:
        enum class EstadoJogo { NoMenu, Jogando, Pausado };
        
        // --- Gerenciamento de Estado ---
        EstadoJogo estadoAtual;
        Gerenciadores::GerenciadorGrafico* pGG;
        Gerenciadores::GerenciadorEventos* pGE; 
        Estados::Menu menu;
        Fases::Fase* pFaseAtual;
        sf::Clock relogio;

        // --- Sistema de Pause e Save ---
        sf::Clock saveClock;
        bool pauseActive{false};
        int pausePos{0};
        bool pauseEnterDebounce{false};
        sf::Font pauseFont;
        // Snapshot em memória para evitar I/O de disco durante o pause
        std::string pauseSnapshot;

        // --- Sistema de Ranking ---
        std::vector<RankingEntry> rankingFase1; 
        std::vector<RankingEntry> rankingFase2;

        // Métodos internos de lógica de loop e persistência
        void processarEventosJogando(); 
        void renderizar();
        void inicializar();
        bool carregarSave();
        void aplicarSnapshotAFase(const std::string& snapshot);
        void processarBlocoEntidade(const std::vector<std::string>& block);
        void salvar();
        void salvarDataBuffer(std::ostream& os);

    public:
        Jogo();
        ~Jogo();

        void executar();

        // Interface pública para o Ranking
        void adicionarAoRanking(int faseNum, std::string nome, int pontuacao);
        const std::vector<RankingEntry>& getRankingFase1() const;
        const std::vector<RankingEntry>& getRankingFase2() const;
};