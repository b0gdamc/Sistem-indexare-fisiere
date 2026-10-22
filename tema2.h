// MIRAUTA BOGDAN CONSTANTIN - 314CB
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

typedef struct File 
{
    char* id;
    int scor;
    char** cuvinte_cheie;
    int nr_cuvinte_cheie;
} File;

typedef struct celula2
{
    File* info;
    struct celula2 *pre, *urm;
} TCelula2, *TL2;

typedef struct ref 
{
    File* fisier;
    struct ref* urm;
} Ref;

typedef struct node {
    char info;
    struct node* left;
    struct node* right;
    Ref *fisiere;
} Node, *Tree;

typedef int (*TFCmp)(File*, File*);

typedef struct Heap {
    int nrMax, nrElem;
    File **v;
    TFCmp comp;
} THeap;

Tree createNode(char data);
void addChild(Tree parent, Tree child);
File* createFile(char* id, int scor, char** cuvinte_cheie, int nr_cuvinte_cheie);
TL2 AlocCelula2(File* f);
Ref* createRef(File* f);
Tree CautaFratele(Tree nod, char litera);
void ADD(TL2* L, Tree root, char* id, int scor, char** cuvinte_cheie, int nr_cuvinte_cheie, FILE* out);
void InserareArbore(Tree* root, char* cuvant, File* fisier);
void DEL(TL2* L, Tree root, char*id, FILE* out);
bool StergeDinArbore(Tree root, char* cuvant, File* fisier);
void FIND(Tree root, char* cuvant, FILE* out);
void ADDKW(Tree root, TL2 L, char* id, char* cuvant, FILE* out);
void DELKW(Tree root, TL2* L, char* id, char* cuvant, FILE* out);
void AfisareArbore(Tree root, char* cuvant, int nivel, FILE* out);
void PRINT(Tree root, FILE* out);
THeap* AlocaHeap(int nrMax, TFCmp comp);
int RelMaxHeap(File* a, File* b);
void DistrugeHeap(THeap **h);
void InsertHeap(THeap* h, File* val);
File* ExtrHeap(THeap* h);
void TOPK(Tree root, char* cuvant, int k, FILE* out);