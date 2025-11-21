#pragma once

#include <vector>
#include <list>
#include <set>
#include <cmath> 

// Forward Declarations
namespace Entidades {
    class Entidade;
    class BolaDeFogo;
    namespace Personagens {
        class Jogador;
        class Inimigo;
    }
    namespace Obstaculos {
        class Obstaculo;
    }
}

namespace Gerenciadores
{
    class GerenciadorColisoes
    {
    private:
        // Atributos seguindo o Modelo UML (STL)
        std::vector<Entidades::Personagens::Inimigo*> LIs;
        std::list<Entidades::Obstaculos::Obstaculo*> LOs;
        std::set<Entidades::BolaDeFogo*> LPs; 

        Entidades::Personagens::Jogador* pJog1;

        // Método auxiliar
        const bool verificarColisao(Entidades::Entidade* pe1, Entidades::Entidade* pe2) const;

        // Métodos de tratamento divididos
        void tratarColisoesJogsObstacs();
        void tratarColisoesJogsInimgs();
        void tratarColisoesJogsProjeteis(); 

    public:
        GerenciadorColisoes();
        ~GerenciadorColisoes();

        // Métodos para popular as listas STL
        void incluirInimigo(Entidades::Personagens::Inimigo* pi);
        void incluirObstaculo(Entidades::Obstaculos::Obstaculo* po);
        void incluirProjetil(Entidades::BolaDeFogo* pj);
        
        void setJogador(Entidades::Personagens::Jogador* pJ);

        void executar(bool resetInimigos = true);
        void limpar(); 
    };
}