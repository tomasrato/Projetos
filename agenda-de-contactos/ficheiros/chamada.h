#ifndef CHAMADA_H
#define CHAMADA_H

#include "avl.h"

#define MAX_NOME 100
#define MAX_NUM 20

typedef struct {
    char chamador[MAX_NOME];
    char destinatario[MAX_NOME];
    char numero[MAX_NUM];
    int duracao;
} Chamada;

const char* IdentifyCaller(const char* numero, AVLTree V);

#endif