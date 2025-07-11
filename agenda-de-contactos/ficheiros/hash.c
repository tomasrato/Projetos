#include "hash.h"
#include "abp.h"
#include "avl.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cores.h"
#include "fatal.h"

#define MinTableSize (10)

Index Hash( char *nome, int TableSize ){
    unsigned int hashVal = 0;
    for (int i = 0; nome[i] != '\0'; i++)
        hashVal = 37*hashVal + nome[i];

    return hashVal % TableSize;
}

HashTable InitializeTable( int TableSize ) {
    HashTable H;
    int i;

    if( TableSize < MinTableSize ){
        Error( "Table size too small" );
        return NULL;
    }

    H = malloc( sizeof( struct HashTbl ) );
    if(!H)
        FatalError( "Out of space!!!\n" );
    
    H->TableSize =  TableSize;

    H->TheLists = malloc( sizeof( Lista ) * TableSize );

    if(!H->TheLists)
        FatalError( "Out of space!!!" );

    for (i = 0; i < H->TableSize; i++)
        H->TheLists[i] = NULL;

    return H;
}

void Normalizar(const char* Entrada, char* Saida) {
    while (*Entrada) {
        unsigned char c = (unsigned char)*Entrada;

        if (c == 0xC3) {  
            Entrada++;
            unsigned char seguinte = *Entrada;

            switch (seguinte) {
                case 0xA1: case 0xA0: case 0xA2: case 0xA3:
                case 0x80: case 0x81: case 0x82: case 0x83:
                    *Saida++ = 'a'; break;
                case 0xA9: case 0xAA: case 0xA8:
                case 0x88: case 0x89: case 0x8A:            
                    *Saida++ = 'e'; break;
                case 0xAD: case 0xAC: case 0xAE:
                case 0x8C: case 0x8D: case 0x8E:           
                    *Saida++ = 'i'; break;
                case 0xB3: case 0xB2: case 0xB4: case 0xB5:
                case 0x92: case 0x93: case 0x94: case 0x95: 
                    *Saida++ = 'o'; break;
                case 0xBA: case 0xB9: case 0xBB:
                case 0x99: case 0x9A: case 0x9B:           
                    *Saida++ = 'u'; break;
                case 0xA7: 
                case 0x87:                                 
                    *Saida++ = 'c'; break;
                default:
                    break;
            }
        } else {
            *Saida++ = tolower(c);
        }

        Entrada++;
    }

    *Saida = '\0';
}

void CreateUniqueName(Contacto* c, HashTable H) {
    char Original[MAX_NOME];
    strcpy(Original, c->nome);

    int indice = 1;

    while (true) {
        bool REP = false;

        for (int i = 0; i < H->TableSize; i++) {
            Lista A = H->TheLists[i];

            while(A) {
                if (strcmp(A->contacto.nome, c->nome) == 0) {
                    snprintf(c->nome, MAX_NOME, "%.*s_%d", MAX_NOME - 12, Original, indice++);
                    REP = true;
                    break;
                }
            
                A = A->Next;
            }

            if (REP)
                break;
        }

        if (!REP)
            break;
    }

}

Contacto* Find(const char* pesquisa, HashTable H) {

    for (int i = 0; i < H->TableSize; i++) {
        Lista A = H->TheLists[i];

        while(A) {

            if (strcmp(A->contacto.nome, pesquisa) == 0) 
                return &A->contacto;
            
            A = A->Next;
        }
    }

    return NULL;
}

bool Insert( Contacto c, HashTable H ){

    CreateUniqueName(&c, H);
    
    int indice = Hash(c.nome, H->TableSize);
    Lista A = H->TheLists[indice];

    Nodo* Novo = malloc(sizeof(Nodo));
    if (!Novo) return false;

    Novo->contacto = c;
    Novo->Next = H->TheLists[indice];
    H->TheLists[indice] = Novo;

    return true;
}

bool Delete(char* nome, HashTable H) {
    int indice = Hash(nome, H->TableSize);
    Lista A = H->TheLists[indice];
    Lista B = NULL;

    while (A) {
        if (strcmp(A->contacto.nome, nome) == 0) {
            if (B)
                B->Next = A->Next;
            else
                H->TheLists[indice] = A->Next;
            free(A);
            return true;
        }
        B = A;
        A = A->Next;
    }

    return false;
}

void DestroyTable( HashTable H ){
    for (int i = 0; i < H->TableSize; i++) {
        Lista A = H->TheLists[i];
        while (A) {
            Lista T = A;
            A = A->Next;
            free(T);
        }
    }
    free( H->TheLists );
    free( H );
}

void PrintTable(HashTable H){
    int capacidade = 100;
    int total = 0;

    Contacto* list = malloc(sizeof(Contacto) * capacidade);

    for (int i = 0; i < H->TableSize; i++) {
        Lista A = H->TheLists[i];
        while (A) {
            if (total == capacidade) {
                capacidade *= 2;
                list = realloc(list, sizeof(Contacto) * capacidade);
            }
            list[total++] = A->contacto;
            A = A->Next;
        }
    }

    char Normalizado1[MAX_NOME], Normalizado2[MAX_NOME];

    for (int i = 0; i < total - 1; i++) {
        for (int j = i + 1; j < total; j++) {
            Normalizar(list[i].nome, Normalizado1);
            Normalizar(list[j].nome, Normalizado2);

            if (strcmp(Normalizado1, Normalizado2) > 0) {
                Contacto temp = list[i];
                list[i] = list[j];
                list[j] = temp;
            }
        }
    }

    for (int i = 0; i < total; i++) {
        printf(BLUE "Nome: " CYAN "%s" BLUE " | Número: " CYAN "%s\n" RESET, list[i].nome, list[i].numero);
    }

    printf("\n");

    free(list);
}