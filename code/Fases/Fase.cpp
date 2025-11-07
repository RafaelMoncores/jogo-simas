#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Entidades/Obstaculos/Parede.hpp"
#include "../Entidades/Personagens/Slime.hpp"
#include <iostream>
#include <string>
#include <random>

namespace Fases
{
    Fase::Fase() :
        jogador1(nullptr)
    {
        if (!texFase.loadFromFile("fase_background.png"))
        {
            std::cerr << "Erro: nao foi possivel carregar imagem fase_background.png\n";
        }
        else
        {
            bgFase.emplace(texFase);
            float scaleX = 1920.f / texFase.getSize().x;
            float scaleY = 1080.f / texFase.getSize().y;
            bgFase->setScale(sf::Vector2f(scaleX, scaleY));
        }

        inicializarUI();
    }

    Fase::~Fase()
    {
        delete jogador1;
        for (listaObstaculos.irParaPrimeiro(); ; listaObstaculos.irParaProximo())
        {
            Entidades::Obstaculos::Obstaculo* pObst = listaObstaculos.getAtual();
            if (pObst == NULL) break; // Condição de parada
            delete pObst;
        }
        for (listaInimigos.irParaPrimeiro(); ; listaInimigos.irParaProximo())
        {
            Entidades::Personagens::Inimigo* pInim = listaInimigos.getAtual();
            if (pInim == NULL) break; // Condição de parada
            delete pInim;
        }
    }

    void Fase::inicializarUI()
    {
        if (!uiFont.openFromFile("PressStart2P-Regular.ttf"))
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
        // --- 1. LIMPEZA ---
        if (jogador1) delete jogador1;
        
        // Deleta todas as entidades da lista principal
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            delete pE;
        }
        
        // Limpa os nós de TODAS as listas
        listaObstaculos.limpar();
        listaInimigos.limpar();
        listaEntidades.limpar(); // Esvazia a lista principal

        // --- 2. CRIAÇÃO DO JOGADOR ---
        Gerenciadores::GerenciadorGrafico::getInstance()->setViewBounds(0.f, 0.f, 1920.f, 1080.f);

        jogador1 = new Entidades::Personagens::Jogador({50.f, 950.0f});
        jogador1->setPosition({50.f, 950.0f});

        // --- 3. PREPARAÇÃO PARA CRIAR ENTIDADES ---
        using Entidades::Obstaculos::Plataforma;
        using Entidades::Obstaculos::Rampa;
        using Entidades::Obstaculos::Parede;
        using Entidades::Personagens::Slime;

        float larguraMundo = 1920.f;
        float alturaMundo = 1080.f;
        float espessuraMuro = 50.f;

        // Paredes
        Parede* pParede1 = new Parede({0.f, 0.f}, {espessuraMuro, alturaMundo}, "terra.png");
        listaObstaculos.incluir(pParede1);
        listaEntidades.incluir(pParede1);

        Parede* pParede2 = new Parede({larguraMundo - espessuraMuro, 0.f}, {espessuraMuro, alturaMundo}, "terra.png");
        listaObstaculos.incluir(pParede2);
        listaEntidades.incluir(pParede2);


        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 1);

        int plat;
        int ramp;
        float cont_plat = 0;
        float cont_ramp_x = 0;
        float cont_ramp_y = 0;

        float alturaSlime = 32.f;

        // --- 4. CRIAÇÃO DAS PLATAFORMAS E INIMIGOS ---

        // aglomerado de plataformas 1
        cont_plat = 0;
        for(plat=0;plat<8;plat++){
            Plataforma* pPlat = new Plataforma({cont_plat, 1050.f}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            cont_plat+=50;
        }

        // aglomerado de plataformas 2
        cont_plat = 400;
        for(plat=0;plat<5;plat++){
            float platY = 950.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            cont_plat+=50;
        }

        // aglomerado de plataformas 3
        cont_plat = 700;
        for(plat=0;plat<2;plat++){
            float platY = 900.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            cont_plat+=50;
        }
        cont_ramp_x = 800;
        cont_ramp_y = 850;
        for(ramp=0;ramp<2;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        Plataforma* pPlat3 = new Plataforma({900.f, 800.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat3);
        listaEntidades.incluir(pPlat3);

        // aglomerado de plataformas 4
        cont_plat=600;
        for(plat=0;plat<4;plat++){
            float platY = 750.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 2 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }

        // aglomerado de plataformas 5
        cont_ramp_x = 400;
        cont_ramp_y = 650;
        for(ramp=0;ramp<3;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, false,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x-=50;
            cont_ramp_y-=50;
        }
        cont_plat=100;
        for(plat=0;plat<4;plat++){
            float platY = 550.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 2 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        Plataforma* pPlat5 = new Plataforma({450.f, 700.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat5);
        listaEntidades.incluir(pPlat5);
        if (dis(gen) == 0)
        {
            Slime* pSlime = new Slime({450.f, 700.f - 2*alturaSlime}, pPlat5);
            listaInimigos.incluir(pSlime);
            listaEntidades.incluir(pSlime);
        }

        // aglomerado de plataformas 6
        cont_plat=350;
        for(plat=0;plat<3;plat++){
            float platY = 450.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 1 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        cont_ramp_x = 500;
        cont_ramp_y = 400;
        for(ramp=0;ramp<2;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        Plataforma* pPlat6 = new Plataforma({600.f, 350.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat6);
        listaEntidades.incluir(pPlat6);
        if (dis(gen) == 0)
        {
            Slime* pSlime = new Slime({600.f, 350.f - 2*alturaSlime}, pPlat6);
            listaInimigos.incluir(pSlime);
            listaEntidades.incluir(pSlime);
        }
            
        // aglomerado de plataformas 7
        cont_plat=200;
        for(plat=0;plat<2;plat++){
            float platY = 200.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 1 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        cont_ramp_x = 400;
        cont_ramp_y = 250;
        for(ramp=0;ramp<2;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, false,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x-=50;
            cont_ramp_y-=50;
        }
        Plataforma* pPlat7 = new Plataforma({450.f, 300.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat7);
        listaEntidades.incluir(pPlat7);
        if (dis(gen) == 0)
        {
            Slime* pSlime = new Slime({450.f, 300.f - 2*alturaSlime}, pPlat7);
            listaInimigos.incluir(pSlime);
            listaEntidades.incluir(pSlime);
        }
            
        // aglomerado de plataformas 8
        cont_plat=300;
        for(plat=0;plat<14;plat++){
            float platY = 50.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 7 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        Rampa* pRamp8 = new Rampa({250, 50}, {50.f, 50.f}, true,"rampa.png");
        listaObstaculos.incluir(pRamp8);
        listaEntidades.incluir(pRamp8);

        // aglomerado de plataformas 9
        cont_plat=1050;
        for(plat=0;plat<5;plat++){
            float platY = 200.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 2 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        cont_ramp_x = 1300;
        cont_ramp_y = 150;
        for(ramp=0;ramp<2;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        Plataforma* pPlat9 = new Plataforma({1450.f, 50.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat9);
        listaEntidades.incluir(pPlat9);
        if (dis(gen) == 0)
        {
            Slime* pSlime = new Slime({1450.f, 50.f - 2*alturaSlime}, pPlat9);
            listaInimigos.incluir(pSlime);
            listaEntidades.incluir(pSlime);
        }
            
        // aglomerado de plataformas 10
        cont_plat=1550;
        for(plat=0;plat<2;plat++){
            float platY = 100.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 1 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }

        // aglomerado de plataformas 11
        cont_plat=1300;
        for(plat=0;plat<5;plat++){
            float platY = 350.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 2 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        cont_ramp_x = 1550;
        cont_ramp_y = 300;
        for(ramp=0;ramp<3;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        Plataforma* pPlat11 = new Plataforma({1700, 200.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat11);
        listaEntidades.incluir(pPlat11);
        if (dis(gen) == 0)
        {
            Slime* pSlime = new Slime({1700.f, 200.f - 2*alturaSlime}, pPlat11);
            listaInimigos.incluir(pSlime);
            listaEntidades.incluir(pSlime);
        }
            
        // aglomerado de plataformas 12
        cont_plat=1550;
        for(plat=0;plat<5;plat++){
            float platY = 500.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 2 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }

        // aglomerado de plataformas 13
        cont_plat=1200;
        for(plat=0;plat<2;plat++){
            float platY = 700.f;
            Plataforma* pPlat = new Plataforma({cont_plat, platY}, {50.f, 50.f},"plataforma.png");
            listaObstaculos.incluir(pPlat);
            listaEntidades.incluir(pPlat);
            if (plat == 1 && dis(gen) == 0)
            {
                Slime* pSlime = new Slime({cont_plat, platY - 2*alturaSlime}, pPlat);
                listaInimigos.incluir(pSlime);
                listaEntidades.incluir(pSlime);
            }
            cont_plat+=50;
        }
        cont_ramp_x = 1300;
        cont_ramp_y = 650;
        for(ramp=0;ramp<2;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x+=100;
            cont_ramp_y-=100;
        }
        Plataforma* pPlat13 = new Plataforma({1450, 550.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat13);
        listaEntidades.incluir(pPlat13);
        if (dis(gen) == 0)
        {
            Slime* pSlime = new Slime({1450.f, 550.f - 2*alturaSlime}, pPlat13);
            listaInimigos.incluir(pSlime);
            listaEntidades.incluir(pSlime);
        }
            
        // aglomerado de plataformas 14
        cont_plat=1900;
        for(plat=0;plat<12;plat++){
            if(plat!=8){
                Plataforma* pPlat = new Plataforma({cont_plat, 1000.f}, {50.f, 50.f},"plataforma.png");
                listaObstaculos.incluir(pPlat);
                listaEntidades.incluir(pPlat);
            }   
            cont_plat-=50;      
        }
        cont_ramp_x = 1300;
        cont_ramp_y = 950;
        for(ramp=0;ramp<2;ramp++){
            Rampa* pRamp = new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, false,"rampa.png");
            listaObstaculos.incluir(pRamp);
            listaEntidades.incluir(pRamp);
            cont_ramp_x-=100;
            cont_ramp_y-=100;
        }
        Plataforma* pPlat14 = new Plataforma({1100, 800.f}, {50.f, 50.f},"plataforma.png");
        listaObstaculos.incluir(pPlat14);
        listaEntidades.incluir(pPlat14);
    }
    
    void Fase::executar(float delta)
    {
        if(jogador1) jogador1->executar(delta);
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->executar(delta); 
        }

        gerenciadorColisoes.verificarColisoes(jogador1, &listaObstaculos, &listaInimigos);

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
            }
        }
        else if (vidasText)
        {
            vidasText->setString("Vidas: 0");
        }
    }

    void Fase::desenhar()
    {
        Gerenciadores::GerenciadorGrafico* pGG = Gerenciadores::GerenciadorGrafico::getInstance();

        pGG->aplicarView();

        // Desenha o fundo
        if (bgFase)
        {
            pGG->desenhar(*bgFase);
        }

        // Desenha as entidades
        if(jogador1) jogador1->desenhar();
        for (listaObstaculos.irParaPrimeiro(); ; listaObstaculos.irParaProximo())
        {
            Entidades::Obstaculos::Obstaculo* pObst = listaObstaculos.getAtual();
            if (pObst == NULL) break;
            pObst->desenhar();
        }
        for (listaInimigos.irParaPrimeiro(); ; listaInimigos.irParaProximo())
        {
            Entidades::Personagens::Inimigo* pInim = listaInimigos.getAtual();
            if (pInim == NULL) break;
            pInim->desenhar();
        }

        pGG->resetarView();

        if (vidasText)
        {
            pGG->desenhar(*vidasText);
        }
    }
}