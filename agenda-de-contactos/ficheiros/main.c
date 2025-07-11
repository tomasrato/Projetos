#include "programa.h"

int main() {
    HashTable H = InitializeTable(TAM_HASH);
    Historico I = CreateList();

    LoadContactos(H);
    LoadHistorico(I);

    MenuInicial(H, I);

    SaveContactos(H);
    SaveHistorico(I);

    DestroyTable(H); 
    DestroyList(I);

    return 0;
}