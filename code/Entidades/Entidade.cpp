#include "Entidade.hpp"

namespace Entidades
{
    Entidade::Entidade() :
        Ente(),
        velocidade({0.0f, 0.0f}),
        G_ACCEL({0.0f, 800.f}),
        x(0),
        y(0),
        buffer(nullptr)
    {
    }

    Entidade::~Entidade()
    {
        // O corpo fica vazio, mas a definição DEVE existir
    }

    void Entidade::salvarDataBuffer()
    {
        if (!buffer) return;

        // Escreve os atributos básicos da Entidade: posição (se disponível) e velocidade
        sf::FloatRect b = getBoundingBox();
        float posX = b.position.x;
        float posY = b.position.y;

        (*buffer) << "Entidade" << std::endl;
        (*buffer) << "posX " << posX << std::endl;
        (*buffer) << "posY " << posY << std::endl;
        (*buffer) << "velX " << velocidade.x << std::endl;
        (*buffer) << "velY " << velocidade.y << std::endl;
    }

    void Entidade::salvar()
    {
        // Chama o método que grava no buffer; espera-se que 'buffer' já tenha sido
        // configurado por quem chama (por exemplo `Fase::salvar`).
        salvarDataBuffer();
    }

    void Entidade::setOutputStream(std::ostream* os)
    {
        buffer = os;
    }
}