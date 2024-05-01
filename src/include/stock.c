#include "../../header/STOCK.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>

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

void ajouterStockMedicament(Stock *s, int *n)
{
    FILE *f = fopen("information/stock.csv", "w");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    s->Medicaments = (medicament *)realloc(s->Medicaments, (*n + 1) * sizeof(medicament));
    if (s->Medicaments == NULL)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    generateUniqueId(s->Medicaments[*n].id);
    remplirMedicament(&s->Medicaments[*n]);

    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie\n");
    for (int i = 0; i < *n; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd\n",
                s->Medicaments[i].nom, s->Medicaments[i].description, s->Medicaments[i].prix,
                s->Medicaments[i].qnt, s->Medicaments[i].seuil_alrt,
                s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a,
                s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a);
    }
    fprintf(f, "%s,%s,%.2f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd\n",
            s->Medicaments[*n].nom, s->Medicaments[*n].description, s->Medicaments[*n].prix,
            s->Medicaments[*n].qnt, s->Medicaments[*n].seuil_alrt,
            s->Medicaments[*n].entre.j, s->Medicaments[*n].entre.m, s->Medicaments[*n].entre.a,
            s->Medicaments[*n].sortie.j, s->Medicaments[*n].sortie.m, s->Medicaments[*n].sortie.a);

    fclose(f);
    (*n)++;
}

void supprimerDernierMedicament(Stock *s, int *n)
{
    if (*n <= 0)
    {
        printf("Aucun Medicament à supprimer.\n");
        return;
    }

    (*n)--;
    s->Medicaments = realloc(s->Medicaments, *n * sizeof(medicament));
    if (s->Medicaments == NULL && *n > 0)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }
}

void modifierMedicament(Stock *s, int n)
{
    if (n <= 0)
    {
        printf("Aucun Medicament à modifier.\n");
        return;
    }

    int index;
    printf("Entrer l'index du Medicament à modifier : ");
    scanf("%d", &index);

    if (index < 0 || index >= n)
    {
        printf("Index invalide.\n");
        return;
    }

    remplirMedicament(&s->Medicaments[index]);
}

void afficherMedicaments(Stock *s, int n)
{
    if (n <= 0)
    {
        printf("Aucun Medicament à afficher.\n");
        return;
    }

    printf("Liste des Medicaments :\n");
    for (int i = 0; i < n; i++)
    {
        printf("ID : %s\n", s->Medicaments[i].id);
        printf("Nom : %s\n", s->Medicaments[i].nom);
        printf("Description : %s\n", s->Medicaments[i].description);
        printf("Prix : %.2f\n", s->Medicaments[i].prix);
        printf("Quantité en stock : %d\n", s->Medicaments[i].qnt);
        printf("Seuil d'alerte : %d\n", s->Medicaments[i].seuil_alrt);
        printf("Date d'entrée en stock : %hd/%hd/%hd\n", s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a);
        printf("Date de sortie du stock : %hd/%hd/%hd\n\n", s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a);
    }
}

void rechercherMedicament(Stock *s, int n)
{
    if (n <= 0)
    {
        printf("Aucun Medicament à rechercher.\n");
        return;
    }

    char nomRecherche[50];
    printf("Entrez le nom du Medicament à rechercher : ");
    scanf("%49s", nomRecherche);

    printf("Résultats de la recherche :\n");
    for (int i = 0; i < n; i++)
    {
        if (strcmp(s->Medicaments[i].nom, nomRecherche) == 0)
        {
            printf("ID : %s\n", s->Medicaments[i].id);
            printf("Nom : %s\n", s->Medicaments[i].nom);
            printf("Description : %s\n", s->Medicaments[i].description);
            printf("Prix : %.2f\n", s->Medicaments[i].prix);
            printf("Quantité en stock : %d\n", s->Medicaments[i].qnt);
            printf("Seuil d'alerte : %d\n", s->Medicaments[i].seuil_alrt);
            printf("Date d'entrée en stock : %hd/%hd/%hd\n", s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a);
            printf("Date de sortie du stock : %hd/%hd/%hd\n\n", s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a);
        }
    }
}

void recherche_de_disponibilite(Stock *s, int n)
{
    char type_car[20];
    int i;
    bool found = false;

    printf("Entrer le type de voiture demandee: ");
    scanf("%s", type_car);

    for (i = 0; i < n; i++)
    {
        if (strcmp(type_car, s->Medicaments[i].nom) == 0)
        {
            found = true;
            if (s->Medicaments[i].qnt >= 1)
            {
                printf("Voiture disponible : %d.\n", s->Medicaments[i].qnt);
            }
            else
            {
                printf("Voiture indisponible.\n");
            }
            break;
        }
    }

    if (!found)
    {
        printf("Type de voiture n'existe pas.\n");
    }
}

void chargerDepuisFichier(Stock *s, int *n)
{
    FILE *f = fopen("information/stock.csv", "r");
    if (f == NULL)
    {
        printf("Échec de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    *n = countLinesInFILE("information/stock.csv");

    // Allocate memory for products
    s->Medicaments = (medicament *)malloc(*n * sizeof(medicament));
    if (s->Medicaments == NULL)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    // Skip the header line
    fscanf(f, "%*[^\n]\n");

    // Read data from file into products
    for (int i = 0; i < *n; i++)
    {
        if (fscanf(f, "%[^,],%[^,],%f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd\n",
                   s->Medicaments[i].nom, s->Medicaments[i].description, &s->Medicaments[i].prix,
                   &s->Medicaments[i].qnt, &s->Medicaments[i].seuil_alrt,
                   &s->Medicaments[i].entre.j, &s->Medicaments[i].entre.m, &s->Medicaments[i].entre.a,
                   &s->Medicaments[i].sortie.j, &s->Medicaments[i].sortie.m, &s->Medicaments[i].sortie.a) != 11)
        {
            printf("Erreur de lecture du fichier.\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(f);
}

void genererBilanCSV(Stock *s, int nbMedicaments)
{
    FILE *f = fopen("information/bilan.csv", "w");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier pour le bilan.\n");
        exit(EXIT_FAILURE);
    }

    float valeurTotale = 0;
    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie\n");
    for (int i = 0; i < nbMedicaments; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d\n",
                s->Medicaments[i].nom, s->Medicaments[i].description, s->Medicaments[i].prix,
                s->Medicaments[i].qnt, s->Medicaments[i].seuil_alrt,
                s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a,
                s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a);
        valeurTotale += s->Medicaments[i].qnt * s->Medicaments[i].prix;
    }

    fprintf(f, "Valeur totale du stock : %.2f\n", valeurTotale);

    fclose(f);
}
