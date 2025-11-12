#pragma once
#include "Elemento.hpp"
#include <cstddef> // Para NULL

namespace Listas{
    // O snippet usa 'TL' como nome do template (Tipo da Lista)
    template <class TL>
    class Lista {
    private:
        // Atributos do snippet
        Elemento<TL>* pPrimeiro;
        Elemento<TL>* pUltimo;

        // Atributo do primeiro diagrama (essencial para iteração)
        Elemento<TL>* pAtual; 

    public:
        
        Lista();
        ~Lista();

        // --- Métodos do Snippet ---
        void incluir(TL* p);
        void limpar();

        // --- MÉTODOS DE ITERAÇÃO (Do primeiro diagrama) ---
        // Essenciais para substituir os loops 'for' do Fase.cpp
        
        // Move o cursor 'pAtual' para o início
        void irParaPrimeiro();
        
        // Avança o cursor 'pAtual'
        void irParaProximo();
        
        // Retorna a informação (pInfo) onde o cursor está
        TL* getAtual() const;
    };

    // --- IMPLEMENTAÇÃO (Template) ---

    template <class TL>
    Lista<TL>::Lista() :
    pPrimeiro(NULL), pUltimo(NULL), pAtual(NULL) {
    }

    template <class TL>
    Lista<TL>::~Lista() {
        limpar();
    }

    template <class TL>
    void Lista<TL>::incluir(TL* p) {
        Elemento<TL>* novoElemento = new Elemento<TL>();
        novoElemento->incluir(p); // Usa o 'incluir' (setInfo) do Elemento

        if (pPrimeiro == NULL) { // Se a lista está vazia
            pPrimeiro = novoElemento;
            pUltimo = novoElemento;
        } else { // Adiciona no final da lista
            pUltimo->setProx(novoElemento);
            pUltimo = novoElemento;
        }
    }

    template <class TL>
    void Lista<TL>::limpar() {
        Elemento<TL>* pAux = pPrimeiro;
        Elemento<TL>* pProximo = NULL;

        while (pAux != NULL) {
            pProximo = pAux->getPRoximo();
        
            delete pAux; 
            pAux = pProximo;
        }

        pPrimeiro = NULL;
        pUltimo = NULL;
        pAtual = NULL;
    }

    // --- Implementação dos Iteradores ---

    template <class TL>
    void Lista<TL>::irParaPrimeiro() {
        pAtual = pPrimeiro;
    }

    template <class TL>
    void Lista<TL>::irParaProximo() {
        if (pAtual != NULL) {
            pAtual = pAtual->getPRoximo();
        }
    }

    template <class TL>
    TL* Lista<TL>::getAtual() const {
        if (pAtual != NULL) {
            return pAtual->getInfo();
        }
        return NULL;
    }

}