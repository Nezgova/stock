#include "../../header/STOCK.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

int countLinesInFILE(const char *filename)
{
    FILE *f = fopen(filename, "r");
    int line = 0;
    char ch;
    if (f == NULL)
    {
        f = fopen(filename, "w");
        assert(f && "Can't open file");
    }
    while ((ch = fgetc(f)) != EOF)
    {
        if (ch == '\n')
        {
            line++;
        }
    }
    fclose(f);
    return line;
}

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

    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie,Type Medicament\n");

    s->Medicaments = (medicament *)realloc(s->Medicaments, (*n + 1) * sizeof(medicament));
    if (s->Medicaments == NULL)
    {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    generateUniqueId(s->Medicaments[*n].id);
    remplirMedicament(&s->Medicaments[*n]);
    for (int i = 0; i < *n; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd,%s\n",
                s->Medicaments[i].nom, s->Medicaments[i].description, s->Medicaments[i].prix,
                s->Medicaments[i].qnt, s->Medicaments[i].seuil_alrt,
                s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a,
                s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a, TypeMedocToString(s->Medicaments[i].Type));
    }

    fprintf(f, "%s,%s,%.2f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd,%s\n",
            s->Medicaments[*n].nom, s->Medicaments[*n].description, s->Medicaments[*n].prix,
            s->Medicaments[*n].qnt, s->Medicaments[*n].seuil_alrt,
            s->Medicaments[*n].entre.j, s->Medicaments[*n].entre.m, s->Medicaments[*n].entre.a,
            s->Medicaments[*n].sortie.j, s->Medicaments[*n].sortie.m, s->Medicaments[*n].sortie.a, TypeMedocToString(s->Medicaments[*n].Type));

    fclose(f);
    (*n)++;
}

void ModificationStockMedicament( int n, int position,medicament m)
{
    // Vérifier si la position est valide
    if (position < 0 || position >= n)
    {
        printf("Position invalide.\n");
        return;
    }

    // Ouvrir le fichier CSV en mode lecture
    FILE *f = fopen("information/stock.csv", "r");
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    // Ouvrir un nouveau fichier temporaire en mode écriture
    FILE *temp = fopen("information/temp.csv", "w");
    if (temp == NULL)
    {
        printf("Erreur lors de la création du fichier temporaire.\n");
        exit(EXIT_FAILURE);
    }

    // Copier les lignes du fichier original vers le fichier temporaire, en sautant la ligne à la position spécifiée
    char ligne[1024];
    int compteur = 0;
    while (fgets(ligne, sizeof(ligne), f) != NULL)
    {
        // Si la ligne est différente de celle à la position spécifiée, la copier dans le fichier temporaire
        if (compteur != position)
        {
            fputs(ligne, temp);
        }
        else{
            
           fprintf(temp, "%s,%s,%.2f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd,%s\n",
                m.nom, m.description, m.prix,
                m.qnt, m.seuil_alrt,
                m.entre.j, m.entre.m, m.entre.a,
                m.sortie.j, m.sortie.m, m.sortie.a, TypeMedocToString(m.Type));
        }
        compteur++;
    }

    // Fermer les fichiers
    fclose(f);
    fclose(temp);

    // Supprimer le fichier original
    if (remove("information/stock.csv") != 0)
    {
        printf("Erreur lors de la suppression du fichier original.\n");
        exit(EXIT_FAILURE);
    }

    // Renommer le fichier temporaire pour remplacer le fichier original
    if (rename("information/temp.csv", "information/stock.csv") != 0)
    {
        printf("Erreur lors du renommage du fichier temporaire.\n");
        exit(EXIT_FAILURE);
    }

   
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
    char type_med[20];
    int i;
    bool found = false;

    printf("Entrer le type de medoc demandee: ");
    scanf("%s", type_med);

    for (i = 0; i < n; i++)
    {
        if (strcmp(type_med, s->Medicaments[i].nom) == 0)
        {
            found = true;
            if (s->Medicaments[i].qnt >= 1)
            {
                printf("medoc disponible : %d.\n", s->Medicaments[i].qnt);
            }
            else
            {
                printf("medoc indisponible.\n");
            }
            break;
        }
    }

    if (!found)
    {
        printf("Type de medoc n'existe pas.\n");
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

    if (s->Medicaments == NULL)
    {
        s->Medicaments = (medicament *)malloc(*n * sizeof(medicament));
    }
    else
    {
        s->Medicaments = realloc(s->Medicaments, *n * sizeof(medicament));
    }

    fscanf(f, "%*[^\n]\n");

    for (int i = 0; i < *n - 1; i++)
    {
        char str[30];
        if (fscanf(f, "%[^,],%[^,],%f,%d,%d,%hd/%hd/%hd,%hd/%hd/%hd,%29s\n",
                   s->Medicaments[i].nom, s->Medicaments[i].description, &s->Medicaments[i].prix,
                   &s->Medicaments[i].qnt, &s->Medicaments[i].seuil_alrt,
                   &s->Medicaments[i].entre.j, &s->Medicaments[i].entre.m, &s->Medicaments[i].entre.a,
                   &s->Medicaments[i].sortie.j, &s->Medicaments[i].sortie.m, &s->Medicaments[i].sortie.a, str) != 12)
        {
            printf("Erreur de lecture du fichier.\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            s->Medicaments[i].Type = StringToTypeMedoc(str);
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
    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie,Type medicament\n");
    for (int i = 0; i < nbMedicaments; i++)
    {
        fprintf(f, "%s,%s,%.2f,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d,%s\n",
                s->Medicaments[i].nom, s->Medicaments[i].description, s->Medicaments[i].prix,
                s->Medicaments[i].qnt, s->Medicaments[i].seuil_alrt,
                s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a,
                s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a, TypeMedocToString(s->Medicaments[i].Type));
        valeurTotale += s->Medicaments[i].qnt * s->Medicaments[i].prix;
    }

    fprintf(f, ",,,,,,Valeur totale du stock : ,%.2f\n", valeurTotale);

    fclose(f);
}
