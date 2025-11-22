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

struct RankingEntry {
    std::string nome;
    int pontuacao;

    // Comparador para ordenar do maior para o menor
    bool operator>(const RankingEntry& other) const {
        return pontuacao > other.pontuacao;
    }
};

class Jogo
{
    private:
        enum class EstadoJogo { NoMenu, Jogando, Pausado };
        
        EstadoJogo estadoAtual;
        Gerenciadores::GerenciadorGrafico* pGG;
        Gerenciadores::GerenciadorEventos* pGE; 
        Estados::Menu menu;
        Fases::Fase* pFaseAtual;
        sf::Clock relogio;

        std::vector<RankingEntry> rankingFase1; 
        std::vector<RankingEntry> rankingFase2;

        void processarEventosJogando(); 
        void renderizar();
        void inicializar();

    public:
        Jogo();
        ~Jogo();

        void executar();

        void adicionarAoRanking(int faseNum, std::string nome, int pontuacao);
        const std::vector<RankingEntry>& getRankingFase1() const;
        const std::vector<RankingEntry>& getRankingFase2() const;
};