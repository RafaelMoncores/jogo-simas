#include "Jogo.hpp"
#include <iostream>
#include <stdexcept>

int main()
{
    // Garante que qualquer falha crítica no jogo seja capturada antes de fechar o programa
    try 
    {
        Jogo jogo_la_em_simao;
        jogo_la_em_simao.executar();
    }
    // Tratamento de exceções padrão do C++ (std::exception e derivadas)
    catch (const std::exception& e)
    {
        std::cerr << "[ERRO CRITICO] O jogo travou: " << e.what() << std::endl;
        return 1; 
    }
    // Tratamento genérico para exceções não padronizadas
    catch (...)
    {
        std::cerr << "[ERRO DESCONHECIDO] Ocorreu um erro nao identificado." << std::endl;
        return 1;
    }

    return 0;
}