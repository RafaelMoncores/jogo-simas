#pragma once
#include <cstddef> 

namespace Listas{
    // Template genérico 'TE' (Tipo do Elemento), permitindo reutilização para qualquer tipo de dado
    template <class TE>
    class Elemento {
    private:
        TE* pInfo;             // Ponteiro para o dado armazenado (ex: um Inimigo, um Obstáculo)
        Elemento<TE>* pProx;   // Ponteiro para o próximo elo da corrente

    public:
        Elemento();
        ~Elemento();

        // Define o dado que este elemento vai segurar
        void incluir(TE* p); 
        
        // Define quem é o próximo elemento na fila
        void setProx(Elemento<TE>* pe);
        
        // Acesso ao próximo elemento (para navegação)
        Elemento<TE>* getPRoximo() const; 

        // Acesso ao dado armazenado
        TE* getInfo() const; 
    };

    // --- IMPLEMENTAÇÃO DO TEMPLATE ---
    
    template <class TE>
    Elemento<TE>::Elemento() :
    pInfo(NULL), pProx(NULL) {
    }

    template <class TE>
    Elemento<TE>::~Elemento() {
        // Não deletamos pInfo aqui. Quem gerencia a vida útil do objeto (Entidade) é a Fase, não a Lista.
    }

    template <class TE>
    void Elemento<TE>::incluir(TE* p) {
        pInfo = p;
    }

    template <class TE>
    void Elemento<TE>::setProx(Elemento<TE>* pe) {
        pProx = pe;
    }

    template <class TE>
    Elemento<TE>* Elemento<TE>::getPRoximo() const {
        return pProx;
    }

    template <class TE>
    TE* Elemento<TE>::getInfo() const {
        return pInfo;
    }
}