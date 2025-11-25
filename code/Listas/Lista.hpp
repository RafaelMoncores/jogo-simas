#pragma once
#include "Elemento.hpp"
#include <cstddef> 

namespace Listas{
    // Template 'TL' define o tipo de dado que a lista vai gerenciar
    template <class TL>
    class Lista {
    private:
        Elemento<TL>* pPrimeiro;
        Elemento<TL>* pUltimo;

        // Cursor interno para permitir iteração externa sem expor os Elementos
        Elemento<TL>* pAtual; 

    public:
        Lista();
        ~Lista();

        void remover(TL* pInfo);
        void incluir(TL* p);
        void limpar();

        // --- Interface de Iteração ---
        // Permite percorrer a lista estilo: irParaPrimeiro() -> while(getAtual()) -> irParaProximo()
        
        void irParaPrimeiro();
        void irParaProximo();
        TL* getAtual() const;
    };

    // --- IMPLEMENTAÇÃO DO TEMPLATE ---

    template <class TL>
    Lista<TL>::Lista() :
    pPrimeiro(NULL), pUltimo(NULL), pAtual(NULL) {
    }

    // Destrutor limpa a estrutura da lista (nós), mas não o conteúdo (pInfo)
    template <class TL>
    Lista<TL>::~Lista() {
        limpar();
    }

    // Adiciona um novo item sempre ao final da lista (Complexidade O(1))
    template <class TL>
    void Lista<TL>::incluir(TL* p) {
        Elemento<TL>* novoElemento = new Elemento<TL>();
        novoElemento->incluir(p); 

        if (pPrimeiro == NULL) { 
            pPrimeiro = novoElemento;
            pUltimo = novoElemento;
        } else { 
            pUltimo->setProx(novoElemento);
            pUltimo = novoElemento;
        }
    }

    // Percorre toda a lista deletando os Elementos (nós), esvaziando a estrutura
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

    // Remove um nó específico baseado no ponteiro de informação
    template <class TL>
    void Lista<TL>::remover(TL* pInfo)
    {
        Elemento<TL>* pAnt = NULL;
        Elemento<TL>* pAux = pPrimeiro;

        // Busca linear pelo elemento a ser removido
        while (pAux != NULL && pAux->getInfo() != pInfo)
        {
            pAnt = pAux;
            pAux = pAux->getPRoximo();
        }

        // Se não encontrou, retorna
        if (pAux == NULL) return;

        // Ajuste de ponteiros (Remoção clássica de Lista Encadeada)
        if (pAux == pPrimeiro)
        {
            pPrimeiro = pAux->getPRoximo();
        }
        else
        {
            pAnt->setProx(pAux->getPRoximo());
        }
        
        if (pAux == pUltimo)
        {
            pUltimo = pAnt;
        }

        // Se o iterador estava no elemento removido, avança para evitar crash
        if (pAux == pAtual)
        {
            pAtual = pAux->getPRoximo();
        }

        delete pAux;
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