#include "Jogo.hpp" // A única inclusão necessária
#include "menu.hpp"

int main()
{
    // Cria o objeto principal do jogo
    //Jogo meuJogo;
    
    // Inicia o loop principal
    //meuJogo.executar();

    Menu menu;
    // novo comentário
    menu.run_menu();

    return 0; // O jogo só chega aqui quando a janela é fechada
}