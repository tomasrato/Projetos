#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "hash.h"
#include "doublelist.h"

void SaveContactos(HashTable H);
void LoadContactos(HashTable H);

void SaveHistorico(Historico I);
void LoadHistorico(Historico I);

#endif