#pragma once
#include "Inimigo.hpp"
#include "Jogador.hpp"
#include "../../Listas/ListaEntidades.hpp"

namespace Entidades
{
    namespace Personagens
    {
        class Dragao : public Inimigo
        {
        private:
            sf::Texture textura;
            short int forca; 

            Listas::ListaEntidades* pListaEntidades;

            bool estaAtacando;
            float temporizadorMovimento;
            float temporizadorAtaque;
            float cooldownTiro;
            int tirosDisparados;
            float tempoMovimentoRestante;
            bool ignorandoObstaculos;

            void atirar();

            virtual void aplicarFisica(float delta) override;

        public:
            Dragao(sf::Vector2f pos, Jogador* pJog1, Jogador* pJog2, Listas::ListaEntidades* pLista);
            Dragao(){};
            ~Dragao();

            virtual void executar(float delta) override;
            virtual void danificar(Personagem* pOutro) override;
            virtual void colidir(Entidade* pOutra, sf::FloatRect boundsOutra) override;

            virtual void salvarDataBuffer() override;
            virtual void salvar() override;
        };
    }
}