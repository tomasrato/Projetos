#ifndef LIST_H
#define LIST_H

#include "chamada.h"

typedef Chamada ElementType2;

typedef struct Node {
    ElementType2 Element;
    struct Node* Next;
    struct Node* Prev;
} Node;

typedef Node* Historico;

Historico CreateList();
void InsertList (Historico I, ElementType2 X);
void InsertByFicheiros(Historico I, ElementType2 X);
void NavegarHistorico(Historico I);
void DestroyList(Historico I);

#endif
