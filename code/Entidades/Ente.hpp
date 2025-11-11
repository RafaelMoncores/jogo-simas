#pragma once
#include "../Gerenciadores/GerenciadorGrafico.hpp"

namespace Entidades
{
    class Ente
    {
        protected:
            static Gerenciadores::GerenciadorGrafico* pGG;
            sf::Drawable* pFig;

        public:
            Ente();
            virtual ~Ente();

            static void setGerenciadorGrafico(Gerenciadores::GerenciadorGrafico* p);

            virtual void executar(float delta) = 0;
            virtual void desenhar() = 0;

            virtual sf::FloatRect getBoundingBox() const = 0;
    };
}