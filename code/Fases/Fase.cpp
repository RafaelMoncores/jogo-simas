#include "Fase.hpp"
#include "../Gerenciadores/GerenciadorGrafico.hpp"
#include "../Entidades/Obstaculos/Parede.hpp"
#include "../Entidades/Personagens/Slime.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

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

        jogador1 = new Entidades::Personagens::Jogador({0.f, 550.0f});
        jogador1->setPosition({0.f, 550.0f});

        construirNivel1();
    }
    
    void Fase::executar(float delta)
    {
        gerenciadorColisoes.verificarColisoes(jogador1, &listaObstaculos, &listaInimigos);

        if(jogador1) jogador1->executar(delta);
        for (listaEntidades.irParaPrimeiro(); ; listaEntidades.irParaProximo())
        {
            Entidades::Entidade* pE = listaEntidades.getAtual();
            if (pE == NULL) break;
            pE->executar(delta); 
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

    void Fase::construirNivel1()
    {
        // --- 1. PREPARAÇÃO ---
    // Declarações 'using' para facilitar a leitura
    using Entidades::Obstaculos::Plataforma;
    using Entidades::Obstaculos::Rampa;
    using Entidades::Obstaculos::Parede;
    using Entidades::Personagens::Slime;

    float larguraMundo = 1900.f; 
    float alturaMundo = 1100.f;  
    float espessuraMuro = 50.f;
    int randP = 0;
    int randR = 0;
    int randW = 0;
    srand(time(nullptr));
    
    // Nomes das texturas (para não repetir strings)
    std::string texPlat = "plataforma.png";
    std::string texRamp = "rampa.png";
    std::string texParede = "terra.png";

    // --- 2. MOLDURA DO MUNDO ---

    Plataforma* pChao1 = new Plataforma({0.f, 600.f}, {300.f, 50.f}, texPlat);
    listaObstaculos.incluir(pChao1);
    listaEntidades.incluir(pChao1);

    Plataforma* pChao2 = new Plataforma({1700.f, 350.f}, {150.f, 50.f}, texPlat);
    listaObstaculos.incluir(pChao2);
    listaEntidades.incluir(pChao2);

    // --- 3. PLATAFORMAS E RAMPAS ---

    randP = rand() % 6 + 1;
    std::cout << randP << std::endl; //debug

    if(randP <= 3){
        Plataforma* pPlat1 = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat1);
        listaEntidades.incluir(pPlat1);

        Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat2);
        listaEntidades.incluir(pPlat2);

        Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat3);
        listaEntidades.incluir(pPlat3);
    }else if(randP == 4){
        Plataforma* pPlat1 = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat1);
        listaEntidades.incluir(pPlat1);

        Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat2);
        listaEntidades.incluir(pPlat2);

        Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat3);
        listaEntidades.incluir(pPlat3);

        Plataforma* pPlat4 = new Plataforma({750.f, 700.f}, {250.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat4);
        listaEntidades.incluir(pPlat4);
    }else if(randP == 5){
        Plataforma* pPlat1 = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat1);
        listaEntidades.incluir(pPlat1);

        Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat2);
        listaEntidades.incluir(pPlat2);

        Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat3);
        listaEntidades.incluir(pPlat3);

        Plataforma* pPlat4 = new Plataforma({750.f, 700.f}, {250.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat4);
        listaEntidades.incluir(pPlat4);

        Plataforma* pPlat5 = new Plataforma({1100.f, 600.f}, {100.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat5);
        listaEntidades.incluir(pPlat5);
    }else{
        Plataforma* pPlat1 = new Plataforma({500.f, 500.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat1);
        listaEntidades.incluir(pPlat1);

        Plataforma* pPlat2 = new Plataforma({1050.f, 350.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat2);
        listaEntidades.incluir(pPlat2);

        Plataforma* pPlat3 = new Plataforma({1450.f, 450.f}, {200.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat3);
        listaEntidades.incluir(pPlat3);

        Plataforma* pPlat4 = new Plataforma({750.f, 700.f}, {250.f, 50.f}, texPlat);
        listaObstaculos.incluir(pPlat4);
        listaEntidades.incluir(pPlat4);

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
    std::cout << randR << std::endl; //debug

    if(randR <= 3){
        Rampa* pRamp1 = new Rampa({800.f, 450.f}, {50.f, 50.f}, false, texRamp);
        listaObstaculos.incluir(pRamp1);
        listaEntidades.incluir(pRamp1);

        Rampa* pRamp2 = new Rampa({950.f, 450.f}, {50.f, 50.f}, true, texRamp);
        listaObstaculos.incluir(pRamp2);
        listaEntidades.incluir(pRamp2);

        Rampa* pRamp3 = new Rampa({1350.f, 500.f}, {50.f, 50.f}, false, texRamp);
        listaObstaculos.incluir(pRamp3);
        listaEntidades.incluir(pRamp3);
    }else if(randR == 4){
        Rampa* pRamp1 = new Rampa({800.f, 450.f}, {50.f, 50.f}, false, texRamp);
        listaObstaculos.incluir(pRamp1);
        listaEntidades.incluir(pRamp1);

        Rampa* pRamp2 = new Rampa({950.f, 450.f}, {50.f, 50.f}, true, texRamp);
        listaObstaculos.incluir(pRamp2);
        listaEntidades.incluir(pRamp2);

        Rampa* pRamp3 = new Rampa({1350.f, 500.f}, {50.f, 50.f}, false, texRamp);
        listaObstaculos.incluir(pRamp3);
        listaEntidades.incluir(pRamp3);

        Rampa* pRamp4 = new Rampa({1300.f, 500.f}, {50.f, 50.f}, true, texRamp);
        listaObstaculos.incluir(pRamp4);
        listaEntidades.incluir(pRamp4);
    }else{
        Rampa* pRamp1 = new Rampa({800.f, 450.f}, {50.f, 50.f}, false, texRamp);
        listaObstaculos.incluir(pRamp1);
        listaEntidades.incluir(pRamp1);

        Rampa* pRamp2 = new Rampa({950.f, 450.f}, {50.f, 50.f}, true, texRamp);
        listaObstaculos.incluir(pRamp2);
        listaEntidades.incluir(pRamp2);

        Rampa* pRamp3 = new Rampa({1350.f, 500.f}, {50.f, 50.f}, false, texRamp);
        listaObstaculos.incluir(pRamp3);
        listaEntidades.incluir(pRamp3);

        Rampa* pRamp4 = new Rampa({1300.f, 500.f}, {50.f, 50.f}, true, texRamp);
        listaObstaculos.incluir(pRamp4);
        listaEntidades.incluir(pRamp4);

        Rampa* pRamp5 = new Rampa({1650.f, 550.f}, {50.f, 50.f}, true, texRamp);
        listaObstaculos.incluir(pRamp5);
        listaEntidades.incluir(pRamp5);
    }

    randW = rand() % 2 + 1;
    std::cout << randW << std::endl; //debug

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
    

    
    
    // --- 4. INIMIGOS ---
    // Nenhum inimigo foi desenhado no mapa.
    // Você pode adicioná-los aqui se desejar, por exemplo:
    /*
    Slime* pSlime1 = new Slime({500.f, 900.f}, pPlat_10_19); // Posição (X=500, Y=900), em cima da pPlat_10_19
    listaInimigos.incluir(pSlime1);
    listaEntidades.incluir(pSlime1);
    */
    }
}