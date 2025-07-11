#ifndef AVL_H
#define AVL_H

#include "contacto.h"
#include "hash.h"

typedef Contacto ElementType;

typedef struct AVLNode {
    Contacto contacto;
    struct AVLNode* Left;
    struct AVLNode* Right;
    int Height;
} *AVLTree;

AVLTree CreateAVL(HashTable H);
AVLTree InsertAVL(Contacto c, AVLTree V);
ElementType* FindByNumero(const char* numero, AVLTree V);
AVLTree MakeAVLEmpty(AVLTree V);

#endif