#include "avl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "fatal.h"
#include "hash.h"


AVLTree CreateAVL(HashTable H) {
    AVLTree Raiz = NULL;

    for (int i = 0; i < H->TableSize; i++) {
        Lista A = H->TheLists[i];
        while (A) {
            Raiz = InsertAVL(A->contacto, Raiz);
            A = A->Next;
        }
    }

    return Raiz;
}

static int Max(int a, int b) {
    if (a > b)
        return a;
    return b;
}

static int Altura(AVLTree V) {
    if (V == NULL)
        return -1;
    return V->Height;
}

static AVLTree RSD(AVLTree V2) {
    AVLTree V1 = V2->Left;
    V2->Left = V1->Right;
    V1->Right = V2;

    V2->Height = Max(Altura(V2->Left), Altura(V2->Right)) + 1;
    V1->Height = Max(Altura(V1->Left), V2->Height) + 1;

    return V1;
}

static AVLTree RSE(AVLTree V2) {
    AVLTree V1 = V2->Right;
    V2->Right = V1->Left;
    V1->Left = V2;

    V2->Height = Max(Altura(V2->Left), Altura(V2->Right)) + 1;
    V1->Height = Max(Altura(V1->Right), V2->Height) + 1;

    return V1;
}

static AVLTree RDDE(AVLTree V3) {
    V3->Right = RSD(V3->Right);
    return RSE(V3);
}

static AVLTree RDED(AVLTree V3) {
    V3->Left = RSE(V3->Left);
    return RSD(V3);
}

AVLTree InsertAVL(ElementType c, AVLTree V) {

    if (V == NULL) {
        V = malloc(sizeof(struct AVLNode));
        if (V == NULL)
            FatalError("Out of Space!!!\n");

    V->contacto = c;
    V->Left = V->Right = NULL;
    V->Height = 0;

    } 

    else if (strcmp(c.numero, V->contacto.numero) < 0) {
        V->Left = InsertAVL(c, V->Left);
        if (Altura(V->Left) - Altura(V->Right) == 2) {
            if (strcmp(c.numero, V->Left->contacto.numero) < 0)
                V = RSD(V);
            else
                V = RDED(V);
        }
    }

    else if (strcmp(c.numero, V->contacto.numero) > 0) {
        V->Right = InsertAVL(c, V->Right);
        if (Altura(V->Right) - Altura(V->Left) == 2) {
            if (strcmp(c.numero, V->Right->contacto.numero) > 0)
                V = RSE(V);
            else
                V = RDDE(V);
        }
    }

    V->Height = Max(Altura(V->Left), Altura(V->Right)) + 1;
    return V;
}

ElementType* FindByNumero(const char* num, AVLTree V) {
    if (V == NULL) 
        return NULL;
    
    int comparar = strcmp(num, V->contacto.numero);

    if (comparar < 0)
        return FindByNumero(num, V->Left);
    else if (comparar > 0)
        return FindByNumero(num, V->Right);
    else
        return &V->contacto;
}

AVLTree MakeAVLEmpty(AVLTree V) {
    if (V != NULL) {
        MakeAVLEmpty(V->Left);
        MakeAVLEmpty(V->Right);
        free(V);
    }
    return NULL;
}

