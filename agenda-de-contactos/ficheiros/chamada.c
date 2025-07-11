#include "chamada.h"
#include <string.h>

const char* IdentifyCaller(const char* numero, AVLTree V) {
    if (strcmp(numero, "OCULTO") == 0) {
        return "UNKNOWN";
    }

    Contacto* c = FindByNumero(numero, V);

    if (c != NULL)
        return c->nome;

    return numero;
}