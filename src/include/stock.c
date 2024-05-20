#include "../../header/STOCK.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define MAX_MEDICAMENTS 10000
#define ID_LENGTH 11
#define NOM_LENGTH 50
#define DESC_LENGTH 100

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"


int countLinesInFILE(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        return 0;
    }
    int line = 0;
    char ch;
    while ((ch = fgetc(f)) != EOF) {
        if (ch == '\n') {
            line++;
        }
    }
    fclose(f);
    return line;
}

void generateUniqueId(char *id) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int longueurCharset = strlen(charset);

    srand((unsigned int)time(NULL));

    for (int i = 0; i < ID_LENGTH - 1; ++i) {
        int index = rand() % longueurCharset;
        id[i] = charset[index];
    }
    id[ID_LENGTH - 1] = '\0';
}

void ecrireDansFichier(const char *filename, Stock *s, int n) {
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", filename);
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie,Type Medicament\n");

    for (int i = 0; i < n; i++) {
        fprintf(f, "%s,%s,%.2f,%d,%d,%02hd/%02hd/%04hd,%02hd/%02hd/%04hd,%s\n",
                s->Medicaments[i].nom, s->Medicaments[i].description, s->Medicaments[i].prix,
                s->Medicaments[i].qnt, s->Medicaments[i].seuil_alrt,
                s->Medicaments[i].entre.j, s->Medicaments[i].entre.m, s->Medicaments[i].entre.a,
                s->Medicaments[i].sortie.j, s->Medicaments[i].sortie.m, s->Medicaments[i].sortie.a, TypeMedocToString(s->Medicaments[i].Type));
    }

    fclose(f);
}

void ajouterStockMedicament(Stock *s, int *n) {
    if (*n >= MAX_MEDICAMENTS) {
        printf("Stock plein, impossible d'ajouter un nouveau médicament.\n");
        return;
    }

    s->Medicaments = (medicament *)realloc(s->Medicaments, (*n + 1) * sizeof(medicament));
    if (s->Medicaments == NULL) {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    generateUniqueId(s->Medicaments[*n].id);
    remplirMedicament(&s->Medicaments[*n]);
    (*n)++;

    ecrireDansFichier("information/stock.csv", s, *n);
}

void ModificationStockMedicament(int n, int position, medicament m) {
    if (position < 0 || position >= n) {
        printf("Position invalide.\n");
        return;
    }

    Stock s;
    s.Medicaments = (medicament *)malloc(n * sizeof(medicament));
    chargerDepuisFichier(&s, &n);
    s.Medicaments[position] = m;

    ecrireDansFichier("information/stock.csv", &s, n);

    free(s.Medicaments);
}

void supprimerStockMedicament(Stock *s, int *n, int position) {
    if (position < 0 || position >= *n) {
        printf("Position invalide.\n");
        return;
    }

    for (int i = position; i < *n - 1; i++) {
        s->Medicaments[i] = s->Medicaments[i + 1];
    }
    s->Medicaments = realloc(s->Medicaments, (*n - 1) * sizeof(medicament));
    if (s->Medicaments == NULL) {
        printf("Allocation de mémoire échouée.\n");
        exit(EXIT_FAILURE);
    }

    (*n)--;

    ecrireDansFichier("information/stock.csv", s, *n);
}

void afficherMedicaments(Stock *s, int n) {
    if (n <= 0) {
        printf("Aucun Medicament à afficher.\n");
        return;
    }

    printf("Liste des Medicaments :\n");
    for (int i = 0; i < n; i++) {
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

void rechercherMedicament(Stock *s, int n) {
    if (n <= 0) {
        printf("Aucun Medicament à rechercher.\n");
        return;
    }

    char nomRecherche[NOM_LENGTH];
    printf("Entrez le nom du Medicament à rechercher : ");
    scanf("%49s", nomRecherche);

    printf("Résultats de la recherche :\n");
    for (int i = 0; i < n; i++) {
        if (strcmp(s->Medicaments[i].nom, nomRecherche) == 0) {
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

void recherche_de_disponibilite(Stock *s, int n) {
    char type_med[NOM_LENGTH];
    bool found = false;

    printf("Entrer le type de medicament demandée: ");
    scanf("%s", type_med);

    for (int i = 0; i < n; i++) {
        if (strcmp(type_med, s->Medicaments[i].nom) == 0) {
            found = true;
            if (s->Medicaments[i].qnt >= 1) {
                printf("Medicament disponible : %d.\n", s->Medicaments[i].qnt);
            } else {
                printf("Medicament indisponible.\n");
            }
            break;
        }
    }

    if (!found) {
        printf("Type de medicament n'existe pas.\n");
    }
}

void chargerDepuisFichier(Stock *s, int *n) {
    FILE *f = fopen("information/stock.csv", "r");
    if (f == NULL) {
        printf("Échec de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    *n = countLinesInFILE("information/stock.csv") - 1; // Subtract 1 for the header line

    if (s->Medicaments == NULL) {
        s->Medicaments = (medicament *)malloc(*n * sizeof(medicament));
    } else {
        s->Medicaments = realloc(s->Medicaments, *n * sizeof(medicament));
    }

    fscanf(f, "%*[^\n]\n"); 

    for (int i = 0; i < *n; i++) {
        char str[30];
        if (fscanf(f, "%[^,],%[^,],%f,%d,%d,%02hd/%02hd/%04hd,%02hd/%02hd/%04hd,%29s\n",
                   s->Medicaments[i].nom, s->Medicaments[i].description, &s->Medicaments[i].prix,
                   &s->Medicaments[i].qnt, &s->Medicaments[i].seuil_alrt,
                   &s->Medicaments[i].entre.j, &s->Medicaments[i].entre.m, &s->Medicaments[i].entre.a,
                   &s->Medicaments[i].sortie.j, &s->Medicaments[i].sortie.m, &s->Medicaments[i].sortie.a, str) != 12) {
            printf("Erreur de lecture du fichier.\n");
            exit(EXIT_FAILURE);
        } else {
            s->Medicaments[i].Type = StringToTypeMedoc(str);
        }
    }

    fclose(f);
}

void genererBilanCSV(Stock *s, int nbMedicaments) {
    ecrireDansFichier("information/bilan.csv", s, nbMedicaments);
}

void afficherGraphiqueQuantites(Stock *s, int n) {
    if (n <= 0) {
        printf("Aucun Medicament à afficher.\n");
        return;
    }

    printf("\nGraphique des quantités de médicaments en stock :\n");
    int maxQuantite = 0;

    for (int i = 0; i < n; i++) {
        if (s->Medicaments[i].qnt > maxQuantite) {
            maxQuantite = s->Medicaments[i].qnt;
        }
    }

    for (int i = 0; i < n; i++) {
        printf("%-20s | ", s->Medicaments[i].nom);
        int barLength = (s->Medicaments[i].qnt * 50) / maxQuantite;
        for (int j = 0; j < barLength; j++) {
            printf("#");
        }
        printf(" %d\n", s->Medicaments[i].qnt);
    }
}


void afficherGraphiqueQuantitesAvance(Stock *s, int n) {
    if (n <= 0) {
        printf("Aucun Medicament à afficher.\n");
        return;
    }

    printf("\nGraphique des quantités de médicaments en stock :\n\n");

    // Trouver la quantité maximale pour échelle
    int maxQuantite = 0;
    for (int i = 0; i < n; i++) {
        if (s->Medicaments[i].qnt > maxQuantite) {
            maxQuantite = s->Medicaments[i].qnt;
        }
    }

    int maxBarWidth = 50; // Largeur maximale de la barre

    // Afficher les barres de l'histogramme avancé avec couleurs
    for (int i = 0; i < n; i++) {
        int barLength = (s->Medicaments[i].qnt * maxBarWidth) / maxQuantite;
        
        // Déterminer la couleur en fonction de la quantité
        char* color;
        if (s->Medicaments[i].qnt < maxQuantite * 0.2) {
            color = RED;
        } else if (s->Medicaments[i].qnt < maxQuantite * 0.4) {
            color = YELLOW;
        } else if (s->Medicaments[i].qnt < maxQuantite * 0.6) {
            color = GREEN;
        } else if (s->Medicaments[i].qnt < maxQuantite * 0.8) {
            color = CYAN;
        } else {
            color = MAGENTA;
        }

        printf("%-20s | %s%-*s%s %d\n", s->Medicaments[i].nom, color, barLength, "##################################################"  + (50 - barLength), RESET, s->Medicaments[i].qnt);
    }
}

void modifierMedicament(Stock *s, int n) {
    if (n <= 0) {
        printf("Aucun Medicament à modifier.\n");
        return;
    }

    int index;
    printf("Entrer l'index du Medicament à modifier : ");
    scanf("%d", &index);

    if (index < 0 || index >= n) {
        printf("Index invalide.\n");
        return;
    }

    remplirMedicament(&s->Medicaments[index]);

    ecrireDansFichier("information/stock.csv", s, n);
}
