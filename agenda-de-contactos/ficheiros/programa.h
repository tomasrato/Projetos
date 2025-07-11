#ifndef PROGRAMA_H
#define PROGRAMA_H

#include "hash.h"
#include "abp.h"
#include "avl.h"
#include "doublelist.h"
#include "chamada.h"
#include "ficheiros.h"
#include "cores.h"

void MenuInicial(HashTable H, Historico I);
void ListarContactos(HashTable H);
void CriarContacto(HashTable H);
void EditarContacto(HashTable H);
void ApagarContacto(HashTable H);
void PesquisarContacto(HashTable H, ABPTree T);
void HistoricoChamadas(Historico I);
void SimularChamada(Historico I, AVLTree V);

#endif