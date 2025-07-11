#include "doublelist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fatal.h"
#include "cores.h"

Historico CreateList() {
    Historico I = malloc(sizeof(Node));
    
    if (!I)
        FatalError("Out of Space!!!\n");

    I->Next = I;
    I->Prev = I;

    return I;
}

void InsertList (Historico I, ElementType2 X) {
    Node* Novo = malloc(sizeof(Node));

    if (!I)
        FatalError("Out of Space!!!\n");

    Novo->Element = X;

    Novo->Next = I->Next;
    Novo->Prev = I;

    I->Next->Prev = Novo;
    I->Next = Novo;

}

void InsertByFicheiros(Historico I, ElementType2 X) {
    Node* Novo = malloc(sizeof(Node));
    if (!Novo)
        FatalError("Out of space!");

    Novo->Element = X;
    Novo->Next = I;
    Novo->Prev = I->Prev;

    I->Prev->Next = Novo;
    I->Prev = Novo;
}

Historico Avancar(Historico P, Historico I) {
    if (P->Next == I)
        return I->Next;
    else
        return P->Next;
}

Historico Retroceder(Historico P, Historico I) {
    if (P->Prev == I)
        return I->Prev;
    else
        return P->Prev;
}

void NavegarHistorico(Historico I) {
    if (I->Next == I) {
        printf(RED "\nHistórico vazio.\n" RESET);
        return;
    }

    Node* cursor = I->Next;
    char opcao;

    while (1) {
        printf(BLUE "\n[Chamador]: " CYAN "%s" BLUE " -> [Destinatário]: " CYAN "%s" BLUE " -> " CYAN "%s" BLUE " | Duração: " CYAN "%d segundos\n" RESET, cursor->Element.chamador, cursor->Element.destinatario, cursor->Element.numero, cursor->Element.duracao);

        printf(BLUE "\nP - Próxima | A - Anterior | S - Sair\n" YELLOW "-> " RESET);
        opcao = getchar();
        while (getchar() != '\n');

        if (opcao == 'S' || opcao == 's') 
            break;

        else if (opcao == 'P' || opcao == 'p') {
            cursor = Avancar(cursor, I);
        } 

        else if (opcao == 'A' || opcao == 'a') {
            cursor = Retroceder(cursor, I);
        } 
        
        else {
            printf(RED "\nOpção inválida!\n" RESET);
        }
    }
}

void DestroyList(Historico I) {
    Node* P = I->Next;
    while (P != I) {
        Node* Temp = P;
        P = P->Next;
        free(Temp);
    }
    free(I);
}