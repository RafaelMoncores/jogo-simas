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
    pGE(nullptr),
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
    pGE = Gerenciadores::GerenciadorEventos::getInstance();
    
    Ente::setGerenciadorGrafico(pGG);

    // Define o Menu como o primeiro ouvinte
    pGE->setOuvinte(&menu);
}

void Jogo::executar()
{
    while (pGG->isWindowOpen())
    {
        pGG->limpar(sf::Color::Black);
        pGE->processarEventos();

        float delta = relogio.restart().asSeconds();

        switch (estadoAtual)
        {
            case EstadoJogo::NoMenu:
            {
                pGE->setOuvinte(&menu);
                int acaoMenu = menu.executar(rankingFase1, rankingFase2);
                Fases::Fase* proximaFase = nullptr;

                switch (acaoMenu)
                {
                    case 0:
                        pGG->fecharWindow();
                        break;
                    case 10:
                        proximaFase = new Fases::FaseUm(this, false);
                        break;
                    case 11:
                        proximaFase = new Fases::FaseUm(this, true);
                        break;
                    case 20:
                        proximaFase = new Fases::FaseDois(this, false);
                        break;
                    case 21:
                        proximaFase = new Fases::FaseDois(this, true);
                        break;
                }

                // Se uma fase foi escolhida, transiciona o estado
                if (proximaFase != nullptr)
                {
                    if (pFaseAtual) { delete pFaseAtual; } // Limpa a fase antiga
                    pFaseAtual = proximaFase;
                    pFaseAtual->inicializar(); // Inicializa a nova
                    estadoAtual = EstadoJogo::Jogando;
                    pGE->setOuvinte(pFaseAtual); // <--- TROCA O OUVINTE PARA A FASE
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
                    pGE->setOuvinte(&menu); // <--- TROCA O OUVINTE DE VOLTA PRO MENU
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
