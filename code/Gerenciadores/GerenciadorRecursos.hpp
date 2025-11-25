#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <iostream>

namespace Gerenciadores {

    class GerenciadorRecursos {
    private:
        static GerenciadorRecursos* pInstancia;
        
        // Cache: Mapa que associa o caminho do arquivo à textura carregada
        std::map<std::string, sf::Texture> mapaTexturas;

        GerenciadorRecursos() {}

    public:
        ~GerenciadorRecursos() {}

        static GerenciadorRecursos* getInstance() {
            if (!pInstancia) pInstancia = new GerenciadorRecursos();
            return pInstancia;
        }

        // Método principal de acesso a assets
        sf::Texture& getTextura(const std::string& caminho) {
            // Verifica se a textura já está em memória para evitar reload (Otimização)
            if (mapaTexturas.find(caminho) == mapaTexturas.end()) {
                sf::Texture tex;
                if (!tex.loadFromFile(caminho)) {
                    std::cerr << "ERRO: Textura nao encontrada: " << caminho << std::endl;
                    throw std::runtime_error("Textura nao encontrada: " + caminho);
                }
                // Armazena no mapa para usos futuros
                mapaTexturas[caminho] = tex;
            }

            return mapaTexturas[caminho];
        }
    };
}