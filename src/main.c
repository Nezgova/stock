#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    short j, m, a;
} date;

typedef struct
{
    int qnt, seuil_alrt;
    char id[10], nom[10], description[100];
    float prix;
    date entre, sortie;
} product;

typedef struct
{
    char user[10];
    product *p;
} stock;

void generateUniqueId(char *id)
{
    int length = 10;
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charsetLength = sizeof(charset) - 1;

    srand((unsigned int)time(NULL)); // Seed the random number generator

    for (int i = 0; i < length; ++i)
    {
        int i = rand() % charsetLength; // Generate a random i within the charset
        id[i] = charset[i];             // Assign a random character from the charset to the ID
    }

    id[length] = '\0'; 
}

void remplissage(stock *s, int *n){

    printf("Entrer le Nom Du Produit\n");
    scanf("%s", s->p[*n].nom);
    printf("Entrer la Description Du Produit\n");
    scanf("%s", s->p[*n].description);
    printf("Entrer le Prix Du Produit\n");
    scanf("%f", &s->p[*n].prix);
    printf("Entrer la Quantite Du Produit dans le stock\n");
    scanf("%d", &s->p[*n].qnt);
    printf("Entrer le Seuil d'alert Du Produit\n");
    scanf("%d", &s->p[*n].seuil_alrt);
    printf("Entrer la Date de dernière entrée en stock du Produit(jour,mois,anne):\n");
    scanf("%d%d%d", &s->p[*n].entre.j, &s->p[*n].entre.m, &s->p[*n].entre.a);
    printf("Entrer la Date de dernière sortie en stock du Produit(jour,mois,anne):\n");
    scanf("%d%d%d", &s->p[*n].sortie.j, &s->p[*n].sortie.m, &s->p[*n].sortie.a);
}

void ajout(stock *s, int *n)
{
    int i;
    FILE *f = fopen("stock.csv", "a");
    s->p = (product *)malloc((*n + 1) * sizeof(product));
    generateUniqueId(s->p[*n].id);
    remplissage(s, n);
    (*n)++;
    for (i = 0; i < *n; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d\n", s->p[i].nom, s->p[i].description, s->p[i].prix, s->p[i].qnt, s->p[i].seuil_alrt, s->p[i].entre.j, s->p[i].entre.m, s->p[i].entre.a, s->p[i].sortie.j, s->p[i].sortie.m, s->p[i].sortie.a);
    }
    fclose(f);
}

void supprimer(stock *s, int *n){
    (*n)--;
    s->p = realloc(s->p, *n *sizeof(product));
}

void loadFromFile(stock *s) {
    FILE *f = fopen("stock.csv", "r");
    if (f == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    int n = 0;
    char line[256];
    while (fgets(line, sizeof(line), f) != NULL) {
        n++;
    }

    s->p = (product *)malloc(n * sizeof(product));
    if (s->p == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }

    fseek(f, 0, SEEK_SET);

    int i = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        sscanf(line, "%[^,],%[^,],%f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd",
               s->p[i].nom, s->p[i].description, &s->p[i].prix,
               &s->p[i].qnt, &s->p[i].seuil_alrt,
               &s->p[i].entre.j, &s->p[i].entre.m, &s->p[i].entre.a,
               &s->p[i].sortie.j, &s->p[i].sortie.m, &s->p[i].sortie.a);
        i++;
    }
    fclose(f);
}

int main()
{

    stock *s;
    int n=0;
    s = (stock *)malloc(sizeof(stock));
    printf("Entrer le nom de l'utilisateur:\n");
    scanf("%s", s->user);
    ajout(s, &n);

    free(s);
    return 0;
}
