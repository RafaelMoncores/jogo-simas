#pragma once
#include <cstddef> // Para NULL

namespace Listas{
    // O snippet usa 'TE' como nome do template (Tipo do Elemento)
    template <class TE>
    class Elemento {
    private:
        TE* pInfo;
        Elemento<TE>* pProx;

    public:
        // Construtor e Destrutor 
        Elemento();
        ~Elemento();

        // Métodos do snippet UML
        void incluir(TE* p); // No snippet, 'incluir' está no Elemento
        void setProx(Elemento<TE>* pe);
        
        // O snippet mostra 'const getPRoximo() const'
        // Interpretado como um método const que retorna um ponteiro
        Elemento<TE>* getPRoximo() const; 

        // Método essencial (implícito por +...() no UML)
        // Necessário para ler a informação de dentro do nó
        TE* getInfo() const; 
    };

    // --- IMPLEMENTAÇÃO (Template) ---
    

    template <class TE>
    Elemento<TE>::Elemento() :
    pInfo(NULL), pProx(NULL) {
        
    }

    template <class TE>
    Elemento<TE>::~Elemento() {
        // O destrutor não deleta 'pInfo'.
        // A Fase é responsável por deletar as Entidades.
    }

    template <class TE>
    void Elemento<TE>::incluir(TE* p) {
        // O método 'incluir' no Elemento 
        // funciona como um 'setInfo'.
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
        // Método adicionado para permitir a leitura do dado
        return pInfo;
    }
}