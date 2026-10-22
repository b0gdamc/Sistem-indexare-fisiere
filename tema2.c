#include "tema2.h"
// fiecare nod este o litera din cuvant
Tree createNode(char data) {
    Tree newnode = malloc(sizeof(Node));
    if(!newnode) return NULL;
    newnode->info = data;
    newnode->left = NULL;   //primul copil
    newnode->right = NULL;  // fratele
    newnode->fisiere = NULL;    //lista de fisiere, doar pentru sf unui cuvant
    return newnode;
}

void addChild(Tree parent, Tree child) {
    if (!parent->left) {
        parent->left = child;
    } else {
        Tree current = parent->left;
        while (current->right) {
            current = current->right;
        }
        current->right = child;
    }
}
// aloca memorie pentru un fisier, aloc dinamic memorie pentru id, cuvinte cheie  
File* createFile(char* id, int scor, char** cuvinte_cheie, int nr_cuvinte_cheie) {
    File* newFile = malloc(sizeof(File));
    if(!newFile) return NULL;

    newFile->id = strdup(id);   // alov memorie pentru id si copiez id ul
    newFile->scor = scor;
    newFile->cuvinte_cheie = (char**)malloc(nr_cuvinte_cheie * sizeof(char*));
    if(!newFile->cuvinte_cheie) {
        free(newFile->id);
        free(newFile);
        return NULL;
    }
    newFile->nr_cuvinte_cheie = nr_cuvinte_cheie;
    // copiez fiecare cuvant cheie in fisier
    for(int i = 0; i < nr_cuvinte_cheie; i++) {
        newFile->cuvinte_cheie[i] = strdup(cuvinte_cheie[i]);
    }

    return newFile;
}
// aloc memorie pentru o celula din lisa dubla inlantuita
TL2 AlocCelula2(File* f) {
    TL2 aux = (TL2)malloc(sizeof(TCelula2));
    if(!aux) return NULL;

    aux->info = f;
    aux->pre = NULL;
    aux->urm = NULL;

    return aux;
}
// aloc un nod pt lista dublu inlantuita care retine referinta catre un fisier
Ref* createRef(File* f) {
    Ref* ref = (Ref*)malloc(sizeof(Ref));
    if(!ref) return NULL;
    ref->fisier = f;
    ref->urm = NULL;
    return ref;
}
// funcitie care cauta un frate cu o anumita litera, daca o gaseste returneaza codul
Tree CautaFratele(Tree nod, char litera) {
    for (Tree temp = nod; temp; temp = temp->right) {
        if (temp->info == litera) {
            return temp;
        }
    }
    return NULL;
}

void ADD(TL2* L, Tree root, char* id, int scor, char** cuvinte_cheie, int nr_cuvinte_cheie, FILE* out) {
    for (TL2 p = *L; p; p = p->urm) {   // verific daca exista deja
        if(strcmp(p->info->id, id) == 0) {
            fprintf(out, "EXISTS\n");
            return;
        }
    }
    // pastrez doar cuvintele cheie unice
    char** cuvant_unic = (char**)malloc(nr_cuvinte_cheie * sizeof(char*));
    int nr_unice = 0;
    
    for(int i = 0; i < nr_cuvinte_cheie; i++) {
        bool gasit = false;
        for(int j = 0; j < nr_unice; j++) {
            if(strcmp(cuvinte_cheie[i], cuvant_unic[j]) == 0) {
                gasit = true;
                break;
            }
        }
        if(!gasit) {
            cuvant_unic[nr_unice] = cuvinte_cheie[i];
            nr_unice++; 
        }
    }
    // creez fisierul si adaug fisierul la finalul liste duble inlt
    File* fisier = createFile(id, scor, cuvant_unic, nr_unice);
    TL2 celula = AlocCelula2(fisier);
    
    if(!*L) {
        *L = celula;
    } else {
        TL2 p = *L;
        while (p->urm) {
            p = p->urm;
        }
        p->urm = celula;
        celula->pre = p;
    }
    // inserez cuvintele unice in arbore
    for(int i = 0; i < nr_unice; i++) {
        InserareArbore(&root, cuvant_unic[i], fisier);
    }
    
    free(cuvant_unic);
    fprintf(out, "OK\n");
}

void InserareArbore(Tree* root, char* cuvant, File* fisier) {
    Tree p = *root;
    // parcurg cuvantul litera cu litera
    for(int i = 0; cuvant[i] != '\0'; i++) {
        char litera = cuvant[i];

        if(!p->left) {  //daca nu are copil, adaug primul copil
            p->left = createNode(litera);
            p = p->left;
        } else {    // caut pozitia alfabetica printre frati
            Tree copil = p->left;
            Tree ant = NULL;
            // avansare prin lista de frati 
            while (copil && copil->info < litera) {
                ant = copil;
                copil = copil->right;
            }
            
            if(copil && copil->info == litera) {
                p = copil;  // daca exisata litera, avansez la copil
            } else {
                Tree nou = createNode(litera);  // inserez litera noua intre ant si copil
                if(!ant) {
                    nou->right = p->left;   //inserare la inceput
                    p->left = nou;
                } else {
                    ant->right = nou;   // inserare dupa
                    nou->right = copil;
                }
                p = nou;
            }
        }
    }
    // am ajuns la finalul cuvantului, adaug fisierul in lista de referinte
    Ref* ref = createRef(fisier);
    if(!p->fisiere || strcmp(fisier->id, p->fisiere->fisier->id) < 0) {
        ref->urm = p->fisiere;
        p->fisiere = ref;
    } else {
        Ref* curent = p->fisiere;
        Ref* ant = NULL;
        while (curent && strcmp(curent->fisier->id, fisier->id) < 0) {
            ant = curent;
            curent = curent->urm;
        }
        ant->urm = ref;
        ref->urm = curent;
    }
}

void DEL(TL2* L, Tree root, char* id, FILE* out) {
    TL2 p = NULL;
    // caut fisierul
    for (p = *L; p; p = p->urm) {
        if(strcmp(p->info->id, id) == 0) {
            break;
        }
    }
    
    if(!p) {
        fprintf(out, "NOT FOUND\n");
        return;
    }
    // sterg fiecare cuvant din arbore
    File* sterg = p->info;
    for(int i = 0; i < sterg->nr_cuvinte_cheie; i++) {
        StergeDinArbore(root, sterg->cuvinte_cheie[i], sterg);
    }
    // refac legaturile din lista dubla inlantuita
    if(p->pre) {
        p->pre->urm = p->urm;
    } else {
        *L = p->urm;
    }
    if(p->urm) {
        p->urm->pre = p->pre;   
    }
    // eliberez memoria
    free(sterg->id);
    for(int i = 0; i < sterg->nr_cuvinte_cheie; i++) {
        free(sterg->cuvinte_cheie[i]);
    }
    free(sterg->cuvinte_cheie);
    free(sterg);
    free(p);
    fprintf(out, "OK\n");
}

bool StergeDinArbore(Tree root, char* cuvant, File* fisier) {
    if(*cuvant == '\0') {   // daca am ajuns la finalul cuvantului
        Ref* ant = NULL;
        // caut referinta catre fisier in lista de ref
        for (Ref* ref = root->fisiere; ref; ant = ref, ref = ref->urm) {
            if(strcmp(ref->fisier->id, fisier->id) == 0) {
                if(!ant) {
                    root->fisiere = ref->urm;
                } else {
                    ant->urm = ref->urm;
                }
                free(ref);
                break;
            }
        }
        if(!root->fisiere && !root->left) {
            return true;    // daca nodul nu mai are nimic, poate fi sters
        }
        return false;
    }
    
    char litera = *cuvant;
    Tree copil = root->left;
    Tree ant = NULL;
    
    while (copil && copil->info != litera) {
        ant = copil;
        copil = copil->right;
    }

    if(copil && copil->info == litera) {    // apelare recursiva pentru urmatoarea litera
        bool sterge = StergeDinArbore(copil, cuvant + 1, fisier);
        if(sterge) {
            if(!ant) {
                root->left = copil->right;
            } else {
                ant->right = copil->right;
            }
            free(copil);
            if(root->info != '\0' && !root->fisiere && !root->left) {
                return true;
            }
        }
    }
    return false;
}

void FIND(Tree root, char* cuvant, FILE* out) {
    Tree p = root;
    for(int i = 0; cuvant[i] != '\0'; i++) {
        Tree copil = CautaFratele(p->left, cuvant[i]);
        if(!copil) {
            fprintf(out, "EMPTY\n");
            return;
        }
        p = copil;
    }   
    
    if(!p->fisiere) {
        fprintf(out, "EMPTY\n");
        return;
    } else {
        int nr_fisiere = 0;
        for (Ref* x = p->fisiere; x; x = x->urm) {
            nr_fisiere++;
        }
        fprintf(out, "%d ", nr_fisiere);

        for (Ref* ref = p->fisiere; ref; ref = ref->urm) {
            fprintf(out, "%s ", ref->fisier->id);
        }
        fprintf(out, "\n");
    }
}

void ADDKW(Tree root, TL2 L, char* id, char* cuvant, FILE* out) {
    TL2 p = NULL;
    for (p = L; p; p = p->urm) {
        if(strcmp(p->info->id, id) == 0) {
            break;
        }
    }
    if(!p) {
        fprintf(out, "NOT FOUND\n");
        return;
    }
    // verific daca exista acel cuvant cheie
    File* fisier = p->info;
    for(int i = 0; i < fisier->nr_cuvinte_cheie; i++) {
        if(strcmp(fisier->cuvinte_cheie[i], cuvant) == 0) {
            fprintf(out, "OK\n");
            return;
        }
    }
    // adaug noul cuvant cheie in fisier
    char** nou = (char**)malloc((fisier->nr_cuvinte_cheie + 1) * sizeof(char*));
    for(int i = 0; i < fisier->nr_cuvinte_cheie; i++) {
        nou[i] = fisier->cuvinte_cheie[i];
    }
    nou[fisier->nr_cuvinte_cheie] = strdup(cuvant);
    free(fisier->cuvinte_cheie);
    fisier->cuvinte_cheie = nou;
    fisier->nr_cuvinte_cheie++;
    
    InserareArbore(&root, cuvant, fisier);
    fprintf(out, "OK\n");
}

void DELKW(Tree root, TL2* L, char* id, char* cuvant, FILE* out) {
    TL2 p = NULL;
    for (p = *L; p; p = p->urm) {
        if(strcmp(p->info->id, id) == 0) {
            break;
        }
    }
    if(!p) {
        fprintf(out, "NOT FOUND\n");
        return;
    }
    
    File* fisier = p->info;
    bool gasit = false;
    for(int i = 0; i < fisier->nr_cuvinte_cheie; i++) {
        if(strcmp(fisier->cuvinte_cheie[i], cuvant) == 0) {
            gasit = true;
            free(fisier->cuvinte_cheie[i]);
            // mut elem la stanga pentru a umple golul
            for(int j = i; j < fisier->nr_cuvinte_cheie - 1; j++) {
                fisier->cuvinte_cheie[j] = fisier->cuvinte_cheie[j + 1];
            }
            break;
        }
    }
    
    if(!gasit) {
        fprintf(out, "OK\n");
        return;
    }
    
    StergeDinArbore(root, cuvant, fisier);
    fisier->nr_cuvinte_cheie--;
    // daca nu mai sunt cuvinte cheie, sterg fisierul
    if(fisier->nr_cuvinte_cheie == 0) {
        if(p->pre) {
            p->pre->urm = p->urm;
        } else {
            *L = p->urm;
        }
        if(p->urm) {
            p->urm->pre = p->pre;
        }
        free(fisier->id);
        free(fisier->cuvinte_cheie);
        free(fisier);
        free(p);
    }
    fprintf(out, "OK\n");
}

void AfisareArbore(Tree root, char* cuvant, int nivel, FILE* out) {
    if(!root) return;
    
    cuvant[nivel] = root->info;
    cuvant[nivel + 1] = '\0';   //terminator de string
    
    if(root->fisiere) {
        int contor = 0;
        for(Ref* ref = root->fisiere; ref; ref = ref->urm) {
            contor++;
        }
        fprintf(out, "%s %d ", cuvant, contor);
        for(Ref* ref = root->fisiere; ref; ref = ref->urm) {
            fprintf(out, "%s ", ref->fisier->id);
        }
        fprintf(out, "\n");
    }
    
    if(root->left) {
        AfisareArbore(root->left, cuvant, nivel + 1, out);
    }
    if(root->right) {
        AfisareArbore(root->right, cuvant, nivel, out);
    }
}

void PRINT(Tree root, FILE* out) {
    char cuvant[105];
    if(root && root->left) {
        AfisareArbore(root->left, cuvant, 0, out);
    } else {
        fprintf(out, "EMPTY\n");
    }
}

THeap* AlocaHeap(int nrMax, TFCmp comp) {
    THeap* h = (THeap*) malloc(sizeof(struct Heap));
    if (!h) return NULL;

    h->v = (File**) malloc(nrMax * sizeof(File*));
    if (!h->v) {
        free(h);
        return NULL;
    }

    h->nrMax = nrMax;
    h->nrElem = 0;
    h->comp = comp;

    return h;
}

int RelMaxHeap(File* a, File* b) {
    if(a->scor > b->scor) {
        return 1;
    }
    if(a->scor == b->scor && strcmp(a->id, b->id) < 0) {
        return 1;
    }
    return 0;
}

void DistrugeHeap(THeap **h) {
    if (*h) {
        free((*h)->v);
        free(*h);
        *h = NULL; 
    }
}

void InsertHeap(THeap* h, File* val) {
    if(!h || h->nrElem == h->nrMax) return;
    
    h->v[h->nrElem] = val;
    h->nrElem++;

    int i = h->nrElem - 1;
    int parinte = (i - 1) / 2;
    
    while(i > 0 && h->comp(h->v[i], h->v[parinte])) { // swap daca fiul este mai mare decat parintele
        File* aux = h->v[i];
        h->v[i] = h->v[parinte];
        h->v[parinte] = aux;
        i = parinte;
        parinte = (i - 1) / 2;
    }
}

File* ExtrHeap(THeap* h) {
    if(!h || h->nrElem == 0) return NULL;
    
    File* elem_extras = h->v[0];
    h->v[0] = h->v[h->nrElem - 1];  // pun ultimul elem in locul radacinii 
    h->nrElem--;
    
    int i = 0;
    while(1) {
        int stanga = 2 * i + 1;
        int dreapta = 2 * i + 2;
        int parinte = i;
        // caut cel mai mare dintre parinti si cei doi fii
        if(stanga < h->nrElem && h->comp(h->v[stanga], h->v[parinte])) {
            parinte = stanga;
        }
        if(dreapta < h->nrElem && h->comp(h->v[dreapta], h->v[parinte])) {
            parinte = dreapta;
        }
        if(parinte == i) break; // elementul este deja in pozitia corecta 
        
        File* aux = h->v[i];
        h->v[i] = h->v[parinte];
        h->v[parinte] = aux;
        i = parinte;
    }
    return elem_extras;
}

void TOPK(Tree root, char* cuvant, int k, FILE* out) {
    Tree p = root;
    // caut cuvantul in arbore
    for(int i = 0; cuvant[i] != '\0'; i++) {
        Tree copil = CautaFratele(p->left, cuvant[i]);
        if(!copil) {
            fprintf(out, "EMPTY\n");
            return;
        }
        p = copil;
    }

    if(!p->fisiere) {
        fprintf(out, "EMPTY\n");
        return;
    }
    
    int nr_fisiere = 0;
    for (Ref* x = p->fisiere; x; x = x->urm) {
        nr_fisiere++;
    }
    // adaug fisierele in heap
    THeap* heap = AlocaHeap(nr_fisiere, RelMaxHeap);
    if(!heap) {
        fprintf(out, "EMPTY\n");
        return;
    }
    
    for (Ref* ref = p->fisiere; ref; ref = ref->urm) {
        InsertHeap(heap, ref->fisier);
    }
    
    if(k > nr_fisiere) {
        k = nr_fisiere; // daca k este mai mare decat nr de fisiere, afisez toate fisierele
    }
    
    fprintf(out, "%d ", k);
    for(int i = 0; i < k; i++) {
        File* fisier = ExtrHeap(heap);
        if(fisier) {
            fprintf(out, "%s ", fisier->id);
        }
    }
    fprintf(out, "\n");
    DistrugeHeap(&heap);
}