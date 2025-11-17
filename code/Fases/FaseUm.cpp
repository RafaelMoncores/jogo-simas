#include "FaseUm.hpp"

#include "../Entidades/Obstaculos/Plataforma.hpp"
#include "../Entidades/Obstaculos/PlataformaFinal.hpp"
#include "../Entidades/Obstaculos/Trampolim.hpp"
#include "../Entidades/Obstaculos/Parede.hpp"
#include "../Entidades/Personagens/Gosma.hpp"
#include "../Entidades/Personagens/Vampiro.hpp"


#include <iostream>
#include <cstdlib>
#include <string>

namespace Fases
{
    FaseUm::FaseUm(Jogo* pJogo) :
        Fase(pJogo, 1),
        pChao2_Gosma(nullptr),
        pPlat1_Gosma(nullptr),
        pPlat4_Gosma(nullptr),
        maxInimMedios(10)
    {
        posJogadorInicial = {0.f, 550.0f};

        if (!texFase.loadFromFile("tileSets/fase1/fase_background.png"))
        {
            std::cerr << "Erro: FaseUm nao carregou background\n";
        }
        else
        {
            bgFase.emplace(texFase);
            float scaleX = 1920.f / texFase.getSize().x;
            float scaleY = 1080.f / texFase.getSize().y;
            bgFase->setScale(sf::Vector2f(scaleX, scaleY));
        }
    }

    FaseUm::~FaseUm()
    {
    }

    void FaseUm::criarObstaculos()
    {
        pChao2_Gosma = nullptr;
        pPlat1_Gosma = nullptr;
        pPlat4_Gosma = nullptr;
    
        using Entidades::Obstaculos::Plataforma;
        using Entidades::Obstaculos::PlataformaFinal;
        using Entidades::Obstaculos::Trampolim;
        using Entidades::Obstaculos::Parede;

        float larguraMundo = 1900.f; 
        float alturaMundo = 1100.f;  
        float espessuraMuro = 50.f;
        int randP = 0;
        int randR = 0;
        int randW = 0;
        srand(time(nullptr));
        
        std::string texPlat = "tileSets/fase1/plataforma.png";
        std::string texTrampolim = "tileSets/trampolim.png";
        std::string texParede = "tileSets/fase1/terra.png";


        Plataforma* pChao1 = new Plataforma({0.f, 600.f}, {300.f, 50.f}, texPlat);
        listaObstaculos.incluir(pChao1);
        listaEntidades.incluir(pChao1); 

        pChao2_Gosma = new PlataformaFinal({1700.f, 350.f}, {150.f, 50.f}, texPlat);
        listaObstaculos.incluir(pChao2_Gosma);
        listaEntidades.incluir(pChao2_Gosma); 

        randP = rand() % 6 + 1;
        std::cout << randP << std::endl;

        if(randP <= 3){
            pPlat1_Gosma = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat1_Gosma);
            listaEntidades.incluir(pPlat1_Gosma);

            Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat2);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat3);
            listaEntidades.incluir(pPlat3);
        }else if(randP == 4){
            pPlat1_Gosma = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat1_Gosma);
            listaEntidades.incluir(pPlat1_Gosma);

            Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat2);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat3);
            listaEntidades.incluir(pPlat3);

            pPlat4_Gosma = new Plataforma({750.f, 700.f}, {250.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat4_Gosma);
            listaEntidades.incluir(pPlat4_Gosma);
        }else if(randP == 5){
            pPlat1_Gosma = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat1_Gosma);
            listaEntidades.incluir(pPlat1_Gosma);

            Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat2);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat3);
            listaEntidades.incluir(pPlat3);

            pPlat4_Gosma = new Plataforma({750.f, 700.f}, {250.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat4_Gosma);
            listaEntidades.incluir(pPlat4_Gosma);

            Plataforma* pPlat5 = new Plataforma({1100.f, 600.f}, {100.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat5);
            listaEntidades.incluir(pPlat5);
        }else{
            pPlat1_Gosma = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat1_Gosma);
            listaEntidades.incluir(pPlat1_Gosma);

            Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat2);
            listaEntidades.incluir(pPlat2);

            Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat3);
            listaEntidades.incluir(pPlat3);

            pPlat4_Gosma = new Plataforma({750.f, 700.f}, {250.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat4_Gosma);
            listaEntidades.incluir(pPlat4_Gosma);

            Plataforma* pPlat5 = new Plataforma({1100.f, 600.f}, {100.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat5);
            listaEntidades.incluir(pPlat5);

            Plataforma* pPlat6 = new Plataforma({1450.f, 650.f}, {200.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat6);
            listaEntidades.incluir(pPlat6);
            Plataforma* pPlat7 = new Plataforma({1600.f, 600.f}, {50.f, 50.f}, texPlat);
            listaObstaculos.incluir(pPlat7);
            listaEntidades.incluir(pPlat7);
        }

        randR = rand() % 5 + 1;
        std::cout << randR << std::endl;

        float forcaRebote = 600.f;

        if(randR <= 3){
            Trampolim* pTramp1 = new Trampolim({800.f, 450.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp1);
            listaEntidades.incluir(pTramp1);

            Trampolim* pTramp2 = new Trampolim({950.f, 450.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp2);
            listaEntidades.incluir(pTramp2);

            Trampolim* pTramp3 = new Trampolim({1350.f, 500.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp3);
            listaEntidades.incluir(pTramp3);
        }else if(randR == 4){
            Trampolim* pTramp1 = new Trampolim({800.f, 450.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp1);
            listaEntidades.incluir(pTramp1);

            Trampolim* pTramp2 = new Trampolim({950.f, 450.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp2);
            listaEntidades.incluir(pTramp2);

            Trampolim* pTramp3 = new Trampolim({1350.f, 500.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp3);
            listaEntidades.incluir(pTramp3);

            Trampolim* pTramp4 = new Trampolim({1300.f, 500.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp4);
            listaEntidades.incluir(pTramp4);
        }else{
            Trampolim* pTramp1 = new Trampolim({800.f, 450.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp1);
            listaEntidades.incluir(pTramp1);

            Trampolim* pTramp2 = new Trampolim({950.f, 450.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp2);
            listaEntidades.incluir(pTramp2);

            Trampolim* pTramp3 = new Trampolim({1350.f, 500.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp3);
            listaEntidades.incluir(pTramp3);

            Trampolim* pTramp4 = new Trampolim({1300.f, 500.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp4);
            listaEntidades.incluir(pTramp4);

            Trampolim* pTramp5 = new Trampolim({1650.f, 550.f}, {50.f, 50.f}, texTrampolim, forcaRebote);
            listaObstaculos.incluir(pTramp5);
            listaEntidades.incluir(pTramp5);
        }

        randW = rand() % 2 + 1;
        std::cout << randW << std::endl;

        if(randW == 1){
            Parede* pParede1 = new Parede({1400.f, 0.f}, {50.f, 350.f}, texParede);
            listaObstaculos.incluir(pParede1);
            listaEntidades.incluir(pParede1);
        }else{
            Parede* pParede1 = new Parede({1400.f, 0.f}, {50.f, 350.f}, texParede);
            listaObstaculos.incluir(pParede1);
            listaEntidades.incluir(pParede1);

            Parede* pParede2 = new Parede({1300.f, 700.f}, {50.f, 400.f}, texParede);
            listaObstaculos.incluir(pParede2);
            listaEntidades.incluir(pParede2);
        }
    }

    void FaseUm::criarInimigos()
    {
        using Entidades::Personagens::Gosma;
        using Entidades::Personagens::Vampiro;

        if (pChao2_Gosma && (rand() % 2 == 0))
        {
            sf::Vector2f pos = {1720.f, 300.f};
            Gosma* pGosma = new Gosma(pos, pChao2_Gosma, jogador1);
            
            listaInimigos.incluir(pGosma);
            listaEntidades.incluir(pGosma);
        }

        if (pPlat1_Gosma) 
        {
            sf::Vector2f pos = {520.f, 450.f};
            Gosma* pGosma = new Gosma(pos, pPlat1_Gosma, jogador1);
            
            listaInimigos.incluir(pGosma);
            listaEntidades.incluir(pGosma);
        }
        
        if (pPlat4_Gosma)
        {
            sf::Vector2f pos = {770.f, 650.f};
            Gosma* pGosma = new Gosma(pos, pPlat4_Gosma, jogador1);
            
            listaInimigos.incluir(pGosma);
            listaEntidades.incluir(pGosma);
        }

        Vampiro* pVamp1 = new Vampiro({100.f, 300.f}, 2.0f, jogador1);
        listaInimigos.incluir(pVamp1);
        listaEntidades.incluir(pVamp1);

        Vampiro* pVamp2 = new Vampiro({1000.f, 250.f}, 2.8f, jogador1);
        listaInimigos.incluir(pVamp2);
        listaEntidades.incluir(pVamp2);
    }

    void FaseUm::criarPlataformas()
    {
        // Método "casca" para cumprir o UML.
        // A lógica principal já está em criarObstaculos().
    }

    void FaseUm::PosarInimigos()
    {
        // Método "casca" para cumprir o UML.
        // A lógica principal já está em criarInimigos().
    }

    void FaseUm::PosarObstaculos()
    {
        // Método "casca" para cumprir o UML.
        // A lógica principal já está em criarObstaculos().
    }

    void FaseUm::criarInimMedios()
    {
        // Método "casca" para cumprir o UML. [cite: 191]
        // (Você pode mover a criação dos Vampiros para cá se quiser)
    }

    void FaseUm::criarObstMedios()
    {
        // Método "casca" para cumprir o UML. [cite: 192]
        // (Você pode mover a criação das Trampolims para cá se quiser)
    }
}