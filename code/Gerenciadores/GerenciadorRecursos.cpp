#include "GerenciadorRecursos.hpp"

namespace Gerenciadores {
    // Sem isso, ocorreria um erro de "undefined reference" no Linker
    GerenciadorRecursos* GerenciadorRecursos::pInstancia = nullptr;
}