#ifndef CONTACTO_H
#define CONTACTO_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NOME 100
#define MAX_NUM 20
#define TAM_HASH 1009

typedef struct {
    char nome[MAX_NOME];
    char numero[MAX_NUM];
} Contacto;

#endif