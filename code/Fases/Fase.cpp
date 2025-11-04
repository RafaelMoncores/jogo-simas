#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Entidades/Obstaculos/Parede.hpp"
#include <iostream>

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
    }

    Fase::~Fase()
    {
        delete jogador1;
        for (auto* pObst : listaObstaculos)
        {
            delete pObst;
        }
    }

    void Fase::inicializar()
    {
        if (jogador1) delete jogador1;
        for (auto* pObst : listaObstaculos) delete pObst;
        listaObstaculos.clear();

        Gerenciadores::GerenciadorGrafico::getInstance()->setViewBounds(0.f, 0.f, 1920.f, 1080.f);

        jogador1 = new Entidades::Personagens::Jogador();
        jogador1->setPosition({50.f, 500.0f});

        using Entidades::Obstaculos::Plataforma;
        using Entidades::Obstaculos::Rampa;
        using Entidades::Obstaculos::Parede;

        float larguraMundo = 1920.f;
        float alturaMundo = 1080.f;
        float espessuraMuro = 50.f;

        listaObstaculos.push_back(new Parede({0.f, 0.f}, {espessuraMuro, alturaMundo}, "terra.png"));
        listaObstaculos.push_back(new Parede({larguraMundo - espessuraMuro, 0.f}, {espessuraMuro, alturaMundo}, "terra.png"));

        int plat;
        int ramp;
        float cont_plat = 0;
        float cont_ramp_x = 0;
        float cont_ramp_y = 0;

        //aglomerado de plataformas 1
        for(plat=0;plat<8;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 1050.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }

        //aglomerado de plataformas 2
        cont_plat = 400;
        for(plat=0;plat<5;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 950.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }

        //aglomerado de plataformas 3
        cont_plat = 700;
        for(plat=0;plat<2;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 900.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        cont_ramp_x = 800;
        cont_ramp_y = 850;
        for(ramp=0;ramp<2;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png"));
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        listaObstaculos.push_back(new Plataforma({900.f, 800.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 4
        cont_plat=600;
        for(plat=0;plat<4;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 750.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }

        //aglomerado de plataformas 5
        cont_ramp_x = 400;
        cont_ramp_y = 650;
        for(ramp=0;ramp<3;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, false,"rampa.png"));
            cont_ramp_x-=50;
            cont_ramp_y-=50;
        }
        cont_plat=100;
        for(plat=0;plat<4;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 550.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        listaObstaculos.push_back(new Plataforma({450.f, 700.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 6
        cont_plat=350;
        for(plat=0;plat<3;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 450.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        cont_ramp_x = 500;
        cont_ramp_y = 400;
        for(ramp=0;ramp<2;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png"));
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        listaObstaculos.push_back(new Plataforma({600.f, 350.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 7
        cont_plat=200;
        for(plat=0;plat<2;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 200.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        cont_ramp_x = 400;
        cont_ramp_y = 250;
        for(ramp=0;ramp<2;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, false,"rampa.png"));
            cont_ramp_x-=50;
            cont_ramp_y-=50;
        }
        listaObstaculos.push_back(new Plataforma({450.f, 300.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 8
        cont_plat=300;
        for(plat=0;plat<14;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 50.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        listaObstaculos.push_back(new Rampa({250, 50}, {50.f, 50.f}, true,"rampa.png"));

        //aglomerado de plataformas 9
        cont_plat=1050;
        for(plat=0;plat<5;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 200.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        cont_ramp_x = 1300;
        cont_ramp_y = 150;
        for(ramp=0;ramp<2;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png"));
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        listaObstaculos.push_back(new Plataforma({1450.f, 50.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 10
        cont_plat=1550;
        for(plat=0;plat<2;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 100.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }

        //aglomerado de plataformas 11
        cont_plat=1300;
        for(plat=0;plat<5;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 350.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        cont_ramp_x = 1550;
        cont_ramp_y = 300;
        for(ramp=0;ramp<3;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png"));
            cont_ramp_x+=50;
            cont_ramp_y-=50;
        }
        listaObstaculos.push_back(new Plataforma({1700, 200.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 12
        cont_plat=1550;
        for(plat=0;plat<5;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 500.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }

        //aglomerado de plataformas 13
        cont_plat=1200;
        for(plat=0;plat<2;plat++){
            listaObstaculos.push_back(new Plataforma({cont_plat, 700.f}, {50.f, 50.f},"plataforma.png"));
            cont_plat+=50;
        }
        cont_ramp_x = 1300;
        cont_ramp_y = 650;
        for(ramp=0;ramp<2;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, true,"rampa.png"));
            cont_ramp_x+=100;
            cont_ramp_y-=100;
        }
        listaObstaculos.push_back(new Plataforma({1450, 550.f}, {50.f, 50.f},"plataforma.png"));

        //aglomerado de plataformas 14
        cont_plat=1900;
        for(plat=0;plat<12;plat++){
            if(plat!=8){
                listaObstaculos.push_back(new Plataforma({cont_plat, 1000.f}, {50.f, 50.f},"plataforma.png"));        
            }   
            cont_plat-=50;        
        }
        cont_ramp_x = 1300;
        cont_ramp_y = 950;
        for(ramp=0;ramp<2;ramp++){
            listaObstaculos.push_back(new Rampa({cont_ramp_x, cont_ramp_y}, {50.f, 50.f}, false,"rampa.png"));
            cont_ramp_x-=100;
            cont_ramp_y-=100;
        }
        listaObstaculos.push_back(new Plataforma({1100, 800.f}, {50.f, 50.f},"plataforma.png"));

        
    }

    void Fase::executar(float delta)
    {
        if(jogador1) jogador1->executar(delta);
        for (auto* pObst : listaObstaculos)
        {
            pObst->executar(delta);
        }

        gerenciadorColisoes.verificarColisoes(jogador1, &listaObstaculos);
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
        for (auto* pObst : listaObstaculos)
        {
            pObst->desenhar();
        }

        pGG->resetarView();
    }
}