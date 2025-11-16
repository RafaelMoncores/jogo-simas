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
                int acaoMenu = menu.executar(ranking);
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
    while (auto event = pGG->pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            pGG->fecharWindow();
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

void Jogo::adicionarAoRanking(std::string nome, int pontuacao)
{
    ranking.push_back({nome, pontuacao});

    // Ordena o vector do maior para o menor
    std::sort(ranking.begin(), ranking.end(), std::greater<RankingEntry>());

    // Mantém apenas os Top 10
    if (ranking.size() > 10)
    {
        ranking.resize(10);
    }
}

const std::vector<RankingEntry>& Jogo::getRanking() const
{
    return ranking;
}
