#pragma once
#include "Ente.hpp"
#include <SFML/Graphics.hpp>
#include <ostream>

namespace Entidades
{
    class Entidade : public Ente
    {
        protected:
            sf::Vector2f velocidade;
            const sf::Vector2f G_ACCEL;
            int x;
            int y;
            std::ostream* buffer;

            virtual void salvarDataBuffer();
            void setOutputStream(std::ostream* os);

        public:
            Entidade();
            virtual ~Entidade();

            virtual void executar(float delta) override = 0;
            virtual void desenhar() override = 0;
            virtual sf::FloatRect getBoundingBox() const override = 0;
            virtual void salvar();
            void setOutputStreamPublic(std::ostream* os) { setOutputStream(os); }
    };
}