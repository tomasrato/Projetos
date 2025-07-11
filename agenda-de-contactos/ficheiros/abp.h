#ifndef BTREE_H
#define BTREE_H

#include "contacto.h"
#include "hash.h"

typedef Contacto ElementType;

struct TreeNode;
typedef struct TreeNode* ABPTree;

ABPTree CreateTree(HashTable H);
ABPTree MakeABPEmpty(ABPTree T);
ABPTree InsertABP(ElementType c, ABPTree T);
void FindABP(const char* t, ABPTree T);

#endif
