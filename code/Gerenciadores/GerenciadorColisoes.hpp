#pragma once

#include <vector>
#include <list>
#include <set>
#include <cmath> 

// Forward Declarations para evitar dependências circulares no header
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
        // Estruturas STL distintas para cada tipo de entidade
        std::vector<Entidades::Personagens::Inimigo*> LIs;
        std::list<Entidades::Obstaculos::Obstaculo*> LOs;
        std::set<Entidades::BolaDeFogo*> LPs; 

        Entidades::Personagens::Jogador* pJog1;

        // Verifica interseção de retângulos (AABB)
        const bool verificarColisao(Entidades::Entidade* pe1, Entidades::Entidade* pe2) const;

        // Separação lógica dos tipos de colisão
        void tratarColisoesJogsObstacs();
        void tratarColisoesJogsInimgs();
        void tratarColisoesJogsProjeteis(); 

    public:
        GerenciadorColisoes();
        ~GerenciadorColisoes();

        // Métodos para registrar entidades na simulação física
        void incluirInimigo(Entidades::Personagens::Inimigo* pi);
        void incluirObstaculo(Entidades::Obstaculos::Obstaculo* po);
        void incluirProjetil(Entidades::BolaDeFogo* pj);
        
        void setJogador(Entidades::Personagens::Jogador* pJ);

        void executar(bool resetInimigos = true);
        void limpar(); 
    };
}