#include "Jogo.hpp"
#include <iostream>
#include <stdexcept> // Necessário para std::exception

int main()
{
    try 
    {
        // Tenta rodar o jogo
        Jogo jogo_la_em_simao;
        jogo_la_em_simao.executar();
    }
    catch (const std::exception& e)
    {
        // Se algo der errado (imagem faltando, erro de lógica), cai aqui
        std::cerr << "[ERRO CRITICO] O jogo travou: " << e.what() << std::endl;
        return 1; // Retorna código de erro para o SO
    }
    catch (...)
    {
        std::cerr << "[ERRO DESCONHECIDO] Ocorreu um erro nao identificado." << std::endl;
        return 1;
    }

    return 0;
}