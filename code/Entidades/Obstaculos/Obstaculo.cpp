#include "Obstaculo.hpp"

namespace Entidades
{
    namespace Obstaculos
    {
        Obstaculo::Obstaculo() :
            Entidade() // Chama o construtor da classe base
        {
        }

        Obstaculo::~Obstaculo()
        {
        }

        // Implementação padrão vazia.
        // A maioria dos obstáculos (como plataformas) é estática
        // e não precisa de lógica de atualização a cada frame.
        void Obstaculo::executar(float delta)
        {
            // não faz nada
        }
    }
}