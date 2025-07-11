#include "abp.h"
#include <stdlib.h>
#include "fatal.h"
#include <string.h>
#include "cores.h"

struct TreeNode{
    ElementType Element;
    ABPTree  Left;
    ABPTree  Right;
};


ABPTree MakeABPEmpty( ABPTree T ){
    if (T != NULL) {
        MakeABPEmpty(T->Left);
        MakeABPEmpty(T->Right);
        free(T);
    }
    return NULL;
}

ABPTree InsertABP(ElementType c, ABPTree T) {
    if (T == NULL) {
        ABPTree Novo = malloc(sizeof(struct TreeNode));
        if (!Novo)
            FatalError("Out of Space!!!\n");
        Novo->Element = c;
        Novo->Left = Novo->Right = NULL;
        return Novo;
    }

    if (strcmp(c.nome, T->Element.nome) < 0)
        T->Left = InsertABP(c, T->Left);
    else if (strcmp(c.nome, T->Element.nome) > 0)
        T->Right = InsertABP(c, T->Right);

    return T;
}


ABPTree CreateTree(HashTable H){
    ABPTree Raiz = NULL;
    for (int i = 0; i < H->TableSize; i++) {
        Lista A = H->TheLists[i];
        while (A) {
            Raiz = InsertABP(A->contacto, Raiz);
            A = A->Next;
        }
    }

    return Raiz;
}


void FindABP(const char* t, ABPTree T) {

    if (T == NULL)
        return;

    char TERMO[100], NOME[100];
    Normalizar(t, TERMO);
    Normalizar(T->Element.nome, NOME);

    if (strstr(NOME, TERMO)) {
        printf(BLUE "Nome -> " CYAN "%s" BLUE " | Número -> " CYAN "%s\n" RESET, T->Element.nome, T->Element.numero);
    }

    FindABP(t, T->Left);
    FindABP(t, T->Right);
}