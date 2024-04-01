#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct
{
    short j, m, a;
} date;

typedef struct
{
    int qnt, seuil_alrt;
    char id[11], nom[50], description[100];
    float prix;
    date entre, sortie;
} product;

typedef struct
{
    char user[50];
    product *produits;
} Stock;

void generateUniqueId(char *id)
{
    int longueur = 10;
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int longueurCharset = strlen(charset);

    srand((unsigned int)time(NULL));

    for (int i = 0; i < longueur; ++i)
    {
        int index = rand() % longueurCharset;
        id[i] = charset[index];
    }

    id[longueur] = '\0';
}

void remplirProduit(product *p)
{
    printf("Entrer le Nom du Produit : ");
    scanf("%49s", p->nom);
    printf("Entrer la Description du Produit : ");
    scanf("%99s", p->description);
    printf("Entrer le Prix du Produit : ");
    scanf("%f", &p->prix);
    printf("Entrer la Quantité du Produit en stock : ");
    scanf("%d", &p->qnt);
    printf("Entrer le Seuil d'alerte du Produit : ");
    scanf("%d", &p->seuil_alrt);
    printf("Entrer la Date de dernière entrée en stock du Produit (jour mois année) : ");
    scanf("%hd%hd%hd", &p->entre.j, &p->entre.m, &p->entre.a);
    printf("Entrer la Date de dernière sortie en stock du Produit (jour mois année) : ");
    scanf("%hd%hd%hd", &p->sortie.j, &p->sortie.m, &p->sortie.a);
}

void ajouterProduit(Stock *s, int *n)
{
    FILE *f = fopen("stock.csv", "a");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    s->produits = (product *)realloc(s->produits, (*n + 1) * sizeof(product));
    if (s->produits == NULL)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    generateUniqueId(s->produits[*n].id);
    remplirProduit(&s->produits[*n]);

    fprintf(f, "%s,%s,%.2f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd\n",
            s->produits[*n].nom, s->produits[*n].description, s->produits[*n].prix,
            s->produits[*n].qnt, s->produits[*n].seuil_alrt,
            s->produits[*n].entre.j, s->produits[*n].entre.m, s->produits[*n].entre.a,
            s->produits[*n].sortie.j, s->produits[*n].sortie.m, s->produits[*n].sortie.a);

    fclose(f);
    (*n)++;
}

void supprimerProduit(Stock *s, int *n)
{
    if (*n <= 0)
    {
        printf("Aucun produit à supprimer.\n");
        return;
    }

    (*n)--;
    s->produits = realloc(s->produits, *n * sizeof(product));
    if (s->produits == NULL && *n > 0)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }
}

void modifierProduit(Stock *s, int n)
{
    if (n <= 0)
    {
        printf("Aucun produit à modifier.\n");
        return;
    }

    int index;
    printf("Entrer l'index du produit à modifier : ");
    scanf("%d", &index);

    if (index < 0 || index >= n)
    {
        printf("Index invalide.\n");
        return;
    }

    remplirProduit(&s->produits[index]);
}

void afficherProduits(Stock *s, int n)
{
    if (n <= 0)
    {
        printf("Aucun produit à afficher.\n");
        return;
    }

    printf("Liste des Produits :\n");
    for (int i = 0; i < n; i++)
    {
        printf("ID : %s\n", s->produits[i].id);
        printf("Nom : %s\n", s->produits[i].nom);
        printf("Description : %s\n", s->produits[i].description);
        printf("Prix : %.2f\n", s->produits[i].prix);
        printf("Quantité en stock : %d\n", s->produits[i].qnt);
        printf("Seuil d'alerte : %d\n", s->produits[i].seuil_alrt);
        printf("Date d'entrée en stock : %hd/%hd/%hd\n", s->produits[i].entre.j, s->produits[i].entre.m, s->produits[i].entre.a);
        printf("Date de sortie du stock : %hd/%hd/%hd\n\n", s->produits[i].sortie.j, s->produits[i].sortie.m, s->produits[i].sortie.a);
    }
}

void rechercherProduit(Stock *s, int n)
{
    if (n <= 0)
    {
        printf("Aucun produit à rechercher.\n");
        return;
    }

    char nomRecherche[50];
    printf("Entrez le nom du produit à rechercher : ");
    scanf("%49s", nomRecherche);

    printf("Résultats de la recherche :\n");
    for (int i = 0; i < n; i++)
    {
        if (strcmp(s->produits[i].nom, nomRecherche) == 0)
        {
            printf("ID : %s\n", s->produits[i].id);
            printf("Nom : %s\n", s->produits[i].nom);
            printf("Description : %s\n", s->produits[i].description);
            printf("Prix : %.2f\n", s->produits[i].prix);
            printf("Quantité en stock : %d\n", s->produits[i].qnt);
            printf("Seuil d'alerte : %d\n", s->produits[i].seuil_alrt);
            printf("Date d'entrée en stock : %hd/%hd/%hd\n", s->produits[i].entre.j, s->produits[i].entre.m, s->produits[i].entre.a);
            printf("Date de sortie du stock : %hd/%hd/%hd\n\n", s->produits[i].sortie.j, s->produits[i].sortie.m, s->produits[i].sortie.a);
        }
    }
}

void chargerDepuisFichier(Stock *s, int *n)
{
    FILE *f = fopen("stock.csv", "r");
    if (f == NULL)
    {
        printf("Échec de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    while (fgetc(f) != EOF)
    {
        (*n)++;
    }

    s->produits = (product *)malloc(*n * sizeof(product));
    if (s->produits == NULL && *n > 0)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    fseek(f, 0, SEEK_SET);

    for (int i = 0; i < *n; i++)
    {
        fscanf(f, "%[^,],%[^,],%f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd\n",
               s->produits[i].nom, s->produits[i].description, &s->produits[i].prix,
               &s->produits[i].qnt, &s->produits[i].seuil_alrt,
               &s->produits[i].entre.j, &s->produits[i].entre.m, &s->produits[i].entre.a,
               &s->produits[i].sortie.j, &s->produits[i].sortie.m, &s->produits[i].sortie.a);
    }

    fclose(f);
}

void genererBilanCSV(Stock *s, int nbProduits)
{
    FILE *f = fopen("bilan.csv", "w");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier pour le bilan.\n");
        exit(EXIT_FAILURE);
    }

    float valeurTotale = 0;
    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie\n");
    for (int i = 0; i < nbProduits; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d\n",
                s->produits[i].nom, s->produits[i].description, s->produits[i].prix,
                s->produits[i].qnt, s->produits[i].seuil_alrt,
                s->produits[i].entre.j, s->produits[i].entre.m, s->produits[i].entre.a,
                s->produits[i].sortie.j, s->produits[i].sortie.m, s->produits[i].sortie.a);
        valeurTotale += s->produits[i].qnt * s->produits[i].prix;
    }

    fprintf(f, "Valeur totale du stock : %.2f\n", valeurTotale);

    fclose(f);
}

int main()
{
    Stock s;
    printf("Entrez le nom de l'utilisateur : ");
    scanf("%49s", s.user);

    int choix;
    int nbProduits = 0;
    s.produits = NULL;

    do
    {
        printf("\nMenu Principal:\n");
        printf("1. Ajouter un produit\n");
        printf("2. Afficher le bilan\n");
        printf("3. Rechercher un produit\n");
        printf("4. Modifier un produit\n");
        printf("5. Supprimer un produit\n");
        printf("6. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            ajouterProduit(&s, &nbProduits);
            break;
        case 2:
            genererBilanCSV(&s, nbProduits);
            printf("Bilan généré avec succès.\n");
            break;
        case 3:
            rechercherProduit(&s, nbProduits);
            break;
        case 4:
            modifierProduit(&s, nbProduits);
            break;
        case 5:
            supprimerProduit(&s, &nbProduits);
            printf("Produit supprimé avec succès.\n");
            break;
        case 6:
            if (s.produits)
            {
                free(s.produits);
            }
            printf("Fermeture...\n");
            break;
        default:
            printf("Choix invalide ! Veuillez réessayer.\n");
        }
    } while (choix != 6);

    if (s.produits)
    {
        free(s.produits);
    }

    printf("Fermeture...\n");
    return 0;
}
