#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

namespace Gerenciadores {

    class GerenciadorRecursos {
    private:
        
        static GerenciadorRecursos* pInstancia;
        
        std::map<std::string, sf::Texture> mapaTexturas;

        GerenciadorRecursos() {}

    public:
        ~GerenciadorRecursos() {}

        static GerenciadorRecursos* getInstance() {
            if (!pInstancia) pInstancia = new GerenciadorRecursos();
            return pInstancia;
        }

        sf::Texture& getTextura(const std::string& caminho) {
            // 1. Verifica se já carregamos essa imagem antes
            if (mapaTexturas.find(caminho) == mapaTexturas.end()) {
                // 2. Se não achou, carrega do disco
                sf::Texture tex;
                if (!tex.loadFromFile(caminho)) {
                    std::cerr << "ERRO: Textura nao encontrada: " << caminho << std::endl;
                    // Aqui entraria seu tratamento de exceção (Item 2)
                    throw std::runtime_error("Textura nao encontrada: " + caminho);
                }
                // 3. Salva no mapa para o futuro
                mapaTexturas[caminho] = tex;
            }

            // 4. Retorna a textura já carregada
            return mapaTexturas[caminho];
        }
    };

}