#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Entidades/BolaDeFogo.hpp"
#include "../Entidades/Personagens/Inimigo.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

namespace Fases
{
    Fase::Fase() :
        Ente(),
        jogador1(nullptr),
        faseConcluida(false),
        posJogadorInicial(0.f, 0.f)
    {
        inicializarUI();
    }

    Fase::~Fase()
    {   
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            delete pE;
        }
        
        listaObstaculos.limpar();
        listaInimigos.limpar();
        listaEntidades.limpar();
        
        jogador1 = nullptr;
    }

    void Fase::inicializarUI()
    {
        if (!uiFont.openFromFile("font/PressStart2P-Regular.ttf"))
        {
            std::cerr << "ERRO: Nao foi possivel carregar a fonte 'PressStart2P-Regular.ttf'" << std::endl;
        }

        vidasText.emplace(uiFont); 
        vidasText->setCharacterSize(30);
        vidasText->setFillColor(sf::Color::White);
        vidasText->setPosition({60.f, 10.f}); 
        vidasText->setString("Vidas: ");
    }
    
    void Fase::inicializar()
    {
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            delete pE;
        }
        
        listaObstaculos.limpar();
        listaInimigos.limpar();
        listaEntidades.limpar();
        
        jogador1 = nullptr;

        Gerenciadores::GerenciadorGrafico::getInstance()->setViewBounds(0.f, 0.f, 1920.f, 1080.f);

        jogador1 = new Entidades::Personagens::Jogador(posJogadorInicial);
        jogador1->setPosition(posJogadorInicial);
        
        listaEntidades.incluir(jogador1); 

        criarObstaculos();
        criarInimigos();
    }
    
    void Fase::executar(float delta)
    {
        gerenciadorColisoes.verificarColisoes(jogador1, &listaObstaculos, &listaInimigos, &listaEntidades);

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->executar(delta); 
        }

        if (jogador1 && jogador1->getCompletouFase())
        {
            faseConcluida = true;
        }

        if (jogador1)
        {
            if (vidasText)
            {
                vidasText->setString("Vidas: " + std::to_string(jogador1->getVidas()));
            }

            if (jogador1->getVidas() <= 0)
            {
                std::cout << "Sem vidas! Reiniciando a fase..." << std::endl;
                inicializar();
                return;
            }
        }
        else if (vidasText)
        {
            vidasText->setString("Vidas: 0");
        }

        bool removeu = true;
        while (removeu)
        {
            removeu = false;
            for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
            {
                Entidades::Entidade* pE = listaEntidades.getAtual();
                if (pE == NULL) break;

                bool deletar = false;

                // Com o include lá em cima, isso agora funciona
                if (Entidades::BolaDeFogo* pFogo = dynamic_cast<Entidades::BolaDeFogo*>(pE))
                {
                    if (!pFogo->getAtivo()) deletar = true;
                }
                else if (Entidades::Personagens::Inimigo* pInim = dynamic_cast<Entidades::Personagens::Inimigo*>(pE))
                {
                    if (pInim->getVidas() <= 0)
                    {
                        deletar = true;
                        listaInimigos.remover(pInim);
                    }
                }
                
                if (deletar)
                {
                    listaEntidades.remover(pE); 
                    delete pE;                  
                    removeu = true;             
                    break; 
                }
            }
        }
    }

    bool Fase::getFaseConcluida() const
    {
        return faseConcluida;
    }

    void Fase::desenhar()
    {
        Gerenciadores::GerenciadorGrafico* pGG = Gerenciadores::GerenciadorGrafico::getInstance();

        pGG->aplicarView();
        if (bgFase) pGG->desenhar(*bgFase);

        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->desenhar();
        }
        
        pGG->resetarView();
        if (vidasText) pGG->desenhar(*vidasText);
    }

    sf::FloatRect Fase::getBoundingBox() const
    {
        return sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}); 
    }

    void Fase::salvar()
    {
        // Método de persistência (ainda não implementado)
    }
}