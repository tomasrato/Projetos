#include "ficheiros.h"
#include <stdio.h>
#include "fatal.h"

void SaveContactos(HashTable H) {
    FILE* f = fopen("CONTACTOS.txt", "w");
    if (!f)
        FatalError("An Error occured while opening the file!!!\n");

    for (int i = 0; i < H->TableSize; i++) {
        Lista A = H->TheLists[i];
        while (A) {
            fprintf(f, "%s;%s\n", A->contacto.nome, A->contacto.numero);
            A = A->Next;
        }
    }

    fclose(f);
}

void LoadContactos(HashTable H) {
    FILE* f = fopen("CONTACTOS.txt", "r");
    if (!f)
        FatalError("An Error occured while opening the file!!!\n");

    Contacto c;
    while (fscanf(f, "%99[^;];%19[^\n]\n", c.nome, c.numero) == 2)
        Insert(c, H);


    fclose(f);
}

void SaveHistorico(Historico I) {
    FILE* f = fopen("HISTORICO.txt", "w");
    if (!f)
        FatalError("An Error occured while opening the file!!!\n");

    Node* P = I->Next;

    while (P != I) {
        fprintf(f, "%s;%s;%s;%i\n", P->Element.chamador, P->Element.destinatario, P->Element.numero, P->Element.duracao);
        P = P->Next;
    }

    fclose(f);

}

void LoadHistorico(Historico I) {
    FILE* f = fopen("HISTORICO.txt", "r");
    if (!f)
        FatalError("An Error occured while opening the file!!!\n");

    Chamada c;

    while (fscanf(f, "%99[^;];%99[^;];%19[^;];%i\n", c.chamador, c.destinatario, c.numero, &c.duracao) == 4) {
        InsertByFicheiros(I, c);
    }
}