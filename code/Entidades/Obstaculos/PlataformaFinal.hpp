#pragma once
#include "Plataforma.hpp"

namespace Entidades
{
    namespace Obstaculos
    {
        class PlataformaFinal : public Plataforma
        {
            public:
                PlataformaFinal(sf::Vector2f pos, sf::Vector2f size, std::string caminhoTextura) :
                    Plataforma(pos, size, caminhoTextura) 
                {
                }
                ~PlataformaFinal() {}
        };
    }
}