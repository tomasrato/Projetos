#ifndef HASH_H
#define HASH_H

#include "contacto.h"

struct AVLNode;
typedef struct AVLNode* AVLTree;

typedef struct Nodo {
    Contacto contacto;
    struct Nodo* Next;
} Nodo;

typedef Nodo* Lista;

typedef struct HashTbl {
    int TableSize;
    Lista* TheLists;
} *HashTable;

typedef unsigned int Index;

void Normalizar(const char* Entrada, char* Saida);
HashTable InitializeTable(int TableSize);
Contacto* Find(const char* pesquisa, HashTable H);
bool Insert(Contacto c, HashTable H);
bool Delete(char* nome, HashTable H);
bool Edit(HashTable H, AVLTree V);
void DestroyTable(HashTable H);
void PrintTable(HashTable H);
void CreateUniqueName(Contacto* c, HashTable H);

#endif