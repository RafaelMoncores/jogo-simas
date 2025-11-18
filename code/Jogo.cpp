#include "Jogo.hpp"
#include <iostream>
#include "Entidades/Ente.hpp"
#include "Entidades/Personagens/Jogador.hpp"

#include "Fases/FaseUm.hpp"
#include "Fases/FaseDois.hpp"

using Gerenciadores::GerenciadorGrafico;
using Entidades::Ente;
using Estados::Menu;
using Fases::Fase;

Jogo::Jogo() :
    pGG(nullptr),
    menu(),
    pFaseAtual(nullptr),
    estadoAtual(EstadoJogo::NoMenu) 
{
    inicializar();
}

Jogo::~Jogo()
{
    delete GerenciadorGrafico::getInstance();
    if(pFaseAtual) delete pFaseAtual;
}

void Jogo::inicializar()
{
    pGG = GerenciadorGrafico::getInstance();
    
    Ente::setGerenciadorGrafico(pGG);
}

void Jogo::executar()
{
    while (pGG->isWindowOpen())
    {
        pGG->limpar(sf::Color::Black);

        float delta = relogio.restart().asSeconds();

        switch (estadoAtual)
        {
            case EstadoJogo::NoMenu:
            {
                int acaoMenu = menu.executar(rankingFase1, rankingFase2);
                if (acaoMenu == 0)
                {
                    pGG->fecharWindow();
                }
                else if (acaoMenu == 1)
                {
                    if(pFaseAtual) { delete pFaseAtual; pFaseAtual = nullptr; }
                    pFaseAtual = new Fases::FaseUm(this);
                    pFaseAtual->inicializar(); 
                    estadoAtual = EstadoJogo::Jogando;
                }
                else if (acaoMenu == 2)
                {
                    if(pFaseAtual) { delete pFaseAtual; pFaseAtual = nullptr; }
                    pFaseAtual = new Fases::FaseDois(this);
                    pFaseAtual->inicializar(); 
                    estadoAtual = EstadoJogo::Jogando;
                }
            }
            break;
        
            case EstadoJogo::Jogando:
            {
                processarEventosJogando();

                if (pFaseAtual)
                {
                    pFaseAtual->executar(delta);
                }

                if (pFaseAtual && pFaseAtual->getFaseConcluida())
                {
                    estadoAtual = EstadoJogo::NoMenu;
                    menu.resetInput();
                    
                    delete pFaseAtual;
                    pFaseAtual = nullptr;
                }
                
                renderizar(); 
            }
            break;

            case EstadoJogo::Pausado:
                // TODO
            break;
        }

        pGG->exibir(); 
    }
}

void Jogo::processarEventosJogando()
{
   while (auto optEvent = pGG->pollEvent())
    {
        const auto& event = *optEvent;

        if (event.is<sf::Event::Closed>())
        {
            pGG->fecharWindow();
        }

        if (pFaseAtual)
        {
            pFaseAtual->processarEvento(event);
        }
    }
}

void Jogo::renderizar()
{
    if (pFaseAtual)
    {
        pFaseAtual->desenhar();
    }
}

void Jogo::adicionarAoRanking(int faseNum, std::string nome, int pontuacao)
{
    // Escolhe o vetor correto
    std::vector<RankingEntry>* pRanking = nullptr;
    if (faseNum == 1) {
        pRanking = &rankingFase1;
    } else if (faseNum == 2) {
        pRanking = &rankingFase2;
    }

    if (!pRanking) return; // Número de fase inválido

    pRanking->push_back({nome, pontuacao});

    // Ordena o vetor (do maior para o menor)
    std::sort(pRanking->begin(), pRanking->end(), std::greater<RankingEntry>());

    // Mantém apenas os Top 10
    if (pRanking->size() > 10)
    {
        pRanking->resize(10);
    }
}

const std::vector<RankingEntry>& Jogo::getRankingFase1() const
{
    return rankingFase1;
}

const std::vector<RankingEntry>& Jogo::getRankingFase2() const
{
    return rankingFase2;
}
