#include "FaseDois.hpp"

#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Entidades/Obstaculos/PlataformaFinal.hpp"
#include "../Entidades/Obstaculos/Trampolim.hpp"
#include "../Entidades/Obstaculos/Parede.hpp"
#include "../Entidades/Personagens/Gosma.hpp"
#include "../Entidades/Personagens/Vampiro.hpp"
#include "../Entidades/Personagens/Dragao.hpp"
#include "../Gerenciadores/GerenciadorRecursos.hpp"

#include <iostream>
#include <cstdlib>
#include <string>

namespace Fases
{
    FaseDois::FaseDois(Jogo* pJogo, bool doisJogadores) :
        Fase(pJogo, 2, doisJogadores),
        pChao_I(nullptr),
        pChao_F(nullptr),
        maxChefoes(1)
    {
        posJogadorInicial = {0.f, 1000.0f};

        if (!texFase.loadFromFile("tileSets/fase2/fase_background.png"))
        {
            std::cerr << "Erro: FaseDois nao carregou background\n";
        }
        else
        {
            bgFase.emplace(texFase);
            float scaleX = 1920.f / texFase.getSize().x;
            float scaleY = 1080.f / texFase.getSize().y;
            bgFase->setScale(sf::Vector2f(scaleX, scaleY));
        }
    }

    FaseDois::FaseDois(): 
        maxChefoes(1)
    {

    }

    FaseDois::~FaseDois()
    {
    }

    void FaseDois::criarObstaculos()
    {
        criarPlataformas();
        criarObstDificil();
    }

    void FaseDois::criarInimigos()
    {
        criarInimigosFaceis();
        criarChefao();
    }

    void FaseDois::criarPlataformas()
    {
        pChao_I = nullptr;
        pChao_F = nullptr;
        
        using Entidades::Obstaculos::Plataforma;
        using Entidades::Obstaculos::PlataformaFinal;

        std::string texPlat = "tileSets/fase2/plataforma.png";
        std::string texPedra = "tileSets/fase2/pedra.png";

        srand(time(nullptr));
        int randP = rand() % 5 + 1;
        std::cout << randP << std::endl;

        pChao_I = new Plataforma({0.f, 1050.f}, {1550.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_I);

        pChao_F = new PlataformaFinal({550.f, 200.f}, {1400.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_F); 
        Plataforma* pPChao_F = new PlataformaFinal({550.f, 250.f}, {1400.f, 50.f}, texPedra);
        listaEntidades.incluir(pPChao_F);

        pChao_1 = new Plataforma({1700.f, 950.f}, {250.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_1);
        Plataforma* pPChao_1 = new PlataformaFinal({1700.f, 1000.f}, {250.f, 50.f}, texPedra);
        listaEntidades.incluir(pPChao_1);

        pChao_2 = new Plataforma({1700.f, 750.f}, {250.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_2);
        Plataforma* pPChao_2 = new PlataformaFinal({1700.f, 800.f}, {250.f, 50.f}, texPedra);
        listaEntidades.incluir(pPChao_2);

        pChao_3 = new Plataforma({0.f, 500.f}, {250.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_3);
        Plataforma* pPChao_3 = new PlataformaFinal({0.f, 550.f}, {250.f, 50.f}, texPedra);
        listaEntidades.incluir(pPChao_3);

        Plataforma* pChao_4 = new Plataforma({0.f, 400.f}, {100.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_4);
        Plataforma* pPChao_4 = new PlataformaFinal({0.f, 450.f}, {100.f, 50.f}, texPedra);
        listaEntidades.incluir(pPChao_4);

        Plataforma* pChao_5 = new Plataforma({0.f, 600.f}, {1150.f, 50.f}, texPlat);
        listaEntidades.incluir(pChao_5);
        Plataforma* pPChao_5 = new PlataformaFinal({0.f, 650.f}, {1150.f, 50.f}, texPedra);
        listaEntidades.incluir(pPChao_5);

        if(randP <= 3){
            Plataforma* pPlat1 = new Plataforma({1450.f, 850.f}, {150.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat1);

            Plataforma* pPlat2 = new Plataforma({1450.f, 650.f}, {150.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({250.f, 300.f}, {100.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat3);
        }else if(randP == 4){
            Plataforma* pPlat1 = new Plataforma({1450.f, 850.f}, {150.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat1);

            Plataforma* pPlat2 = new Plataforma({1450.f, 650.f}, {150.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({250.f, 300.f}, {100.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat3);

            Plataforma* pPlat4 = new Plataforma({400.f, 950.f}, {150.f, 44.f}, texPlat);
            listaEntidades.incluir(pPlat4);


        }else{
            Plataforma* pPlat1 = new Plataforma({1450.f, 850.f}, {150.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat1);

            Plataforma* pPlat2 = new Plataforma({1450.f, 650.f}, {150.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({250.f, 300.f}, {100.f, 50.f}, texPlat);
            listaEntidades.incluir(pPlat3);

            Plataforma* pPlat4 = new Plataforma({400.f, 950.f}, {150.f, 44.f}, texPlat);
            listaEntidades.incluir(pPlat4);

            Plataforma* pPlat5 = new Plataforma({850.f, 950.f}, {100.f, 44.f}, texPlat);
            listaEntidades.incluir(pPlat5);
        }
    }

    void FaseDois::criarObstDificil()
    {
        using Entidades::Obstaculos::Parede;
        std::string texParede = "tileSets/fase2/parede.png";
        srand(time(nullptr));
        int randW = rand() % 4 + 1;
        std::cout << randW << std::endl;

        if(randW <= 3){
            Parede* pParede1 = new Parede({950.f, 300.f}, {50.f, 200.f}, texParede);
            listaEntidades.incluir(pParede1);

            Parede* pParede2 = new Parede({1050.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede2);

            Parede* pParede3 = new Parede({750.f, 300.f}, {50.f, 200.f}, texParede);
            listaEntidades.incluir(pParede3);

            Parede* pParede4 = new Parede({850.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede4);

            Parede* pParede5 = new Parede({650.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede5);
        }else if(randW <= 2){
            Parede* pParede1 = new Parede({950.f, 300.f}, {50.f, 200.f}, texParede);
            listaEntidades.incluir(pParede1);

            Parede* pParede2 = new Parede({1050.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede2);

            Parede* pParede3 = new Parede({750.f, 300.f}, {50.f, 200.f}, texParede);
            listaEntidades.incluir(pParede3);

            Parede* pParede4 = new Parede({850.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede4);
        }else{
            Parede* pParede1 = new Parede({950.f, 300.f}, {50.f, 200.f}, texParede);
            listaEntidades.incluir(pParede1);

            Parede* pParede2 = new Parede({1050.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede2);

            Parede* pParede4 = new Parede({850.f, 550.f}, {50.f, 50.f}, texParede);
            listaEntidades.incluir(pParede4);
        }
    }

    void FaseDois::criarInimigosFaceis()
    {
        using Entidades::Personagens::Gosma;

        int randG = 0;
        srand(time(nullptr));

        randG = rand() % 5 + 1;
        std::cout << randG << std::endl;

        if(randG <= 3){
            sf::Vector2f pos1 = {1750.f, 900.f};
            Gosma* pGosma1 = new Gosma(pos1, pChao_1, jogador1, jogador2);   
            listaEntidades.incluir(pGosma1);

            sf::Vector2f pos2 = {1750.f, 700.f};
            Gosma* pGosma2 = new Gosma(pos2, pChao_2, jogador1, jogador2);   
            listaEntidades.incluir(pGosma2);

            sf::Vector2f pos3 = {150.f, 450.f};
            Gosma* pGosma3 = new Gosma(pos3, pChao_3, jogador1, jogador2);   
            listaEntidades.incluir(pGosma3);

            sf::Vector2f pos4 = {650.f, 950.f};
            Gosma* pGosma4 = new Gosma(pos4, pChao_I, jogador1, jogador2);   
            listaEntidades.incluir(pGosma4);

            sf::Vector2f pos5 = {1050.f, 950.f};
            Gosma* pGosma5 = new Gosma(pos5, pChao_I, jogador1, jogador2);   
            listaEntidades.incluir(pGosma5);
        }else if(randG == 4){
            sf::Vector2f pos1 = {1750.f, 900.f};
            Gosma* pGosma1 = new Gosma(pos1, pChao_1, jogador1, jogador2);   
            listaEntidades.incluir(pGosma1);

            sf::Vector2f pos2 = {1750.f, 700.f};
            Gosma* pGosma2 = new Gosma(pos2, pChao_2, jogador1, jogador2);   
            listaEntidades.incluir(pGosma2);

            sf::Vector2f pos4 = {650.f, 950.f};
            Gosma* pGosma4 = new Gosma(pos4, pChao_I, jogador1, jogador2);   
            listaEntidades.incluir(pGosma4);

            sf::Vector2f pos5 = {1050.f, 950.f};
            Gosma* pGosma5 = new Gosma(pos5, pChao_I, jogador1, jogador2);   
            listaEntidades.incluir(pGosma5);
        }else{
            sf::Vector2f pos1 = {1750.f, 900.f};
            Gosma* pGosma1 = new Gosma(pos1, pChao_1, jogador1, jogador2);   
            listaEntidades.incluir(pGosma1);

            sf::Vector2f pos4 = {650.f, 950.f};
            Gosma* pGosma4 = new Gosma(pos4, pChao_I, jogador1, jogador2);
            listaEntidades.incluir(pGosma4);

            sf::Vector2f pos5 = {1050.f, 950.f};
            Gosma* pGosma5 = new Gosma(pos5, pChao_I, jogador1, jogador2);   
            listaEntidades.incluir(pGosma5);
        }
    }

    void FaseDois::criarChefao()
    {
        using Entidades::Personagens::Dragao;

        sf::Vector2f posDragao = {1000.f, 100.f};
        Dragao* pDragao = new Dragao(posDragao, jogador1, jogador2, &listaEntidades);
        listaEntidades.incluir(pDragao);
    }

    void FaseDois::PosarInimigos()
    {
        // Para load
    }

    void FaseDois::PosarObstaculos()
    {
        // Para load
    }
}