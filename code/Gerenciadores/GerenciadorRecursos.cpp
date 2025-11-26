#include "GerenciadorRecursos.hpp"

namespace Gerenciadores {
    // Sem isso, ocorre um erro de "undefined reference" no Linker
    GerenciadorRecursos* GerenciadorRecursos::pInstancia = nullptr;
}