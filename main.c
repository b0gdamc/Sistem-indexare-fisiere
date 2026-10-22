// MIRAUTA BOGDAN CONSTANTIN - 314CB
#include "tema2.h"

int main()
{
    FILE* in = fopen("indexare.in", "r");
    FILE* out = fopen("indexare.out", "w");

    if(!in || !out) {
        printf("Eroare la deschiderea fisiserelor!\n");
        if(in) fclose(in);
        if(out) fclose(out);
        return 1;
    }

    TL2 L = NULL;
    Tree root = createNode('\0');

    int Q;
    fscanf(in, "%d", &Q);

    char comanda[20];

    for(int i = 0; i < Q; i++)
    {
        fscanf(in, "%s", comanda);

        if(strcmp(comanda, "ADD") == 0)
        {
            char id[100];
            int scor, nr_cuvinte_cheie;
            fscanf(in, "%s %d %d", id, &scor, &nr_cuvinte_cheie);

            char** cuvinte = (char**)malloc(nr_cuvinte_cheie * sizeof(char*));
            char sir[100];
            for(int j = 0; j < nr_cuvinte_cheie; j++)
            {
                fscanf(in, "%s", sir);
                cuvinte[j] = strdup(sir);
            }
            ADD(&L, root, id, scor, cuvinte, nr_cuvinte_cheie, out);
            for(int j = 0; j < nr_cuvinte_cheie; j++)
            {
                free(cuvinte[j]);
            }
            free(cuvinte);
        }
        else if(strcmp(comanda, "DEL") == 0)
        {
            char id[100];
            fscanf(in, "%s", id);
            DEL(&L, root, id, out);
        }
        else if(strcmp(comanda, "FIND") == 0)
        {
            char cuvant[100];
            fscanf(in, "%s", cuvant);
            FIND(root, cuvant, out);
        }
        else if(strcmp(comanda, "ADDKW") == 0)
        {
            char id[100];
            char cuvant[100];
            fscanf(in, "%s %s", id, cuvant);
            ADDKW(root, L, id, cuvant, out);
        }
        else if(strcmp(comanda, "DELKW") == 0)
        {
            char id[100];
            char cuvant[100];
            fscanf(in, "%s %s", id, cuvant);
            DELKW(root, &L, id, cuvant, out);
        }
        else if(strcmp(comanda, "PRINT") == 0)
        {
            PRINT(root, out);
        }
        else if(strcmp(comanda, "TOPK") == 0)
        {
            char cuvant[100];
            int k;
            fscanf(in, "%s %d", cuvant, &k);
            TOPK(root, cuvant, k, out);
        }
    }
    fclose(in);
    fclose(out);

    return 0;
}