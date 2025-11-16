#pragma once

#include "Gerenciadores/GerenciadorGrafico.hpp"
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
        Estados::Menu menu;
        Fases::Fase* pFaseAtual;
        sf::Clock relogio;

        std::vector<RankingEntry> ranking;

        void processarEventosJogando(); 
        void renderizar();
        void inicializar();

    public:
        Jogo();
        ~Jogo();

        void executar();

        void adicionarAoRanking(std::string nome, int pontuacao);
        const std::vector<RankingEntry>& getRanking() const;
};