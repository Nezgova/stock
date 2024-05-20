#include "../../header/MEDICAMENT.h"
#include "../../header/STOCK.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

const char* TypeMedocStrings[] = {
    TYPE_MEDOC_LIST(STRING_DEF)
};

void remplirMedicament(medicament *p) {
    printf("Entrer le Nom du medicament : ");
    scanf("%49s", p->nom);
    rewind(stdin);

    printf("Entrer la Description du medicament : ");
    scanf("%99s", p->description);
    rewind(stdin);

    do {
        printf("Entrer le Prix du medicament : ");
        rewind(stdin);
    } while (scanf("%f", &p->prix) != 1 || p->prix <= 0);

    do {
        printf("Entrer la Quantité du medicament en stock : ");
        rewind(stdin);
    } while (scanf("%d", &p->qnt) != 1 || p->qnt < 0);

    do {
        printf("Entrer le Seuil d'alerte du medicament : ");
        rewind(stdin);
    } while (scanf("%d", &p->seuil_alrt) != 1 || p->seuil_alrt < 0);

    do {
        printf("Entrer la Date de dernière entrée en stock du medicament (jour mois année) : ");
        rewind(stdin);
    } while (scanf("%hd%hd%hd", &p->entre.j, &p->entre.m, &p->entre.a) != 3);

    do {
        printf("Entrer la Date de dernière sortie en stock du medicament (jour mois année) : ");
        rewind(stdin);
    } while (scanf("%hd%hd%hd", &p->sortie.j, &p->sortie.m, &p->sortie.a) != 3);

    printf("Voulez-vous entrer le type du medicament comme une chaine de caractères ? (Oui/Non) : ");
    char response[4];
    scanf("%3s", response);
    rewind(stdin);

    bool useString = (strcmp(response, "Oui") == 0 || strcmp(response, "oui") == 0);

    for (int i = 0; i < TYPE_COUNT; ++i) {
        printf("%d. %s\n", i, TypeMedocStrings[i]);
    }

    if (useString) {
        char typeStr[20];
        do {
            printf("Entrer le Type du medicament : ");
            scanf("%19s", typeStr);
            rewind(stdin);
            p->Type = StringToTypeMedoc(typeStr);
        } while (p->Type == TYPE_COUNT);
    } else {
        int typeNum;
        do {
            printf("Entrer le Type du medicament (0-5) : ");
            scanf("%d", &typeNum);
            rewind(stdin);
            p->Type = (TypeMedoc)typeNum;
        } while (typeNum < 0 || typeNum >= TYPE_COUNT);
    }
}

void achat(medicament *m, int n) {
    FILE *f = fopen("information/ticket.csv", "w");
    FILE *history = fopen("information/history.csv", "a");

    if (f == NULL || history == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    fprintf(f, "Nom du Medicament,Quantite,Prix,type\n");
    float total = 0;

    while (true) {
        printf("Entrer :\n1.Pour Acheter.\n2.Pour Quitter.\n");
        int choix;
        scanf("%d", &choix);
        if (choix == 2) break;

        printf("Entrer le nom du medicament : ");
        char nom_med[20];
        scanf("%s", nom_med);

        printf("Entrer la quantite de votre medicament : ");
        int qt_med;
        scanf("%d", &qt_med);

        bool found = false;
        for (int i = 0; i < n; i++) {
            if (strcmp(nom_med, m[i].nom) == 0) {
                found = true;
                if (qt_med > m[i].qnt) {
                    printf("Quantite insuffisante au stock\n");
                } else {
                    m[i].qnt -= qt_med;
                    if (m[i].qnt <= m[i].seuil_alrt) {
                        printf("Veuillez recharger le stock, il reste %d medicament nomme %s\n", m[i].qnt, m[i].nom);
                    }
                    printf("ACHAT REUSSI\n");
                    ModificationStockMedicament(n, i, m[i]);
                    fprintf(f, "%s,%d,%.2f,%s\n", nom_med, qt_med, qt_med * m[i].prix, TypeMedocToString(m[i].Type));
                    fprintf(history, "%s,%d,%.2f,%s\n", nom_med, qt_med, qt_med * m[i].prix, TypeMedocToString(m[i].Type));
                    total += qt_med * m[i].prix;
                }
                break;
            }
        }
        if (!found) {
            printf("Medicament non trouvé\n");
        }
    }

    fprintf(f, "TOTAL,%.2fDH\n", total);
    fclose(f);
    fclose(history);
}

void chargerDepuisFichierHistory(medicament** meds, int* n) {
    FILE *f = fopen("information/history.csv", "r");
    if (f == NULL) {
        printf("Échec de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }

    int lineCount = countLinesInFILE("information/history.csv");
    *n = lineCount;

    if (*n <= 0) {
        fclose(f);
        *meds = NULL;
        return;
    }

    *meds = (medicament *)malloc(*n * sizeof(medicament));
    if (*meds == NULL) {
        printf("Erreur de mémoire.\n");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < *n; i++) {
        if (fscanf(f, "%[^,],%d,%f,%[^\n]\n", (*meds)[i].nom, &(*meds)[i].qnt, &(*meds)[i].prix, (*meds)[i].description) != 4) {
            printf("Erreur lors de la lecture de la ligne %d.\n", i + 1);
            free(*meds);
            fclose(f);
            exit(EXIT_FAILURE);
        }
    }

    fclose(f);
}

void affichage_history(medicament *m, int n) {
    for (int i = 0; i < n; i++) {
        printf("Nom: %s, Quantité: %d, Prix: %.2f, Description: %s\n", m[i].nom, m[i].qnt, m[i].prix, m[i].description);
    }
}

void statistics(medicament* m, int choix) {
    while (choix != 4) {
        int or = 0, inj = 0, perf = 0, vac = 0, ant = 0, des = 0;
        int n = countLinesInFILE("information/history.csv");

        switch (choix) {
            case 1:
                for (int i = 0; i < n; i++) {
                    if (strcmp(m[i].description, "ORAUX") == 0) or++;
                    else if (strcmp(m[i].description, "INJECTABLES") == 0) inj++;
                    else if (strcmp(m[i].description, "PERFUSION") == 0) perf++;
                    else if (strcmp(m[i].description, "VACCINS") == 0) vac++;
                    else if (strcmp(m[i].description, "ANTISEPTIQUES") == 0) ant++;
                    else if (strcmp(m[i].description, "DESINFECTANTS") == 0) des++;
                }
                int t[6] = {or, inj, perf, vac, ant, des};
                int max = 0;
                for (int i = 0; i < 6; i++) {
                    if (max < t[i]) {
                        max = t[i];
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (t[i] == max) {
                        switch (i) {
                            case 0: printf("Le type du medicament le plus achete est : ORAUX\n"); break;
                            case 1: printf("Le type du medicament le plus achete est : INJECTABLES\n"); break;
                            case 2: printf("Le type du medicament le plus achete est : PERFUSION\n"); break;
                            case 3: printf("Le type du medicament le plus achete est : VACCINS\n"); break;
                            case 4: printf("Le type du medicament le plus achete est : ANTISEPTIQUES\n"); break;
                            case 5: printf("Le type du medicament le plus achete est : DESINFECTANTS\n"); break;
                        }
                    }
                }
                break;
            case 2: {
                float totalRevenue = 0;
                for (int i = 0; i < n; i++) {
                    totalRevenue += m[i].prix;
                }
                printf("Le Revenu total de l'entreprise d'apres les medicaments est : %.2fDH\n", totalRevenue);
                break;
            }
            case 3: {
                float revenues[6] = {0};
                for (int i = 0; i < n; i++) {
                    if (strcmp(m[i].description, "ORAUX") == 0) revenues[0] += m[i].prix;
                    else if (strcmp(m[i].description, "INJECTABLES") == 0) revenues[1] += m[i].prix;
                    else if (strcmp(m[i].description, "PERFUSION") == 0) revenues[2] += m[i].prix;
                    else if (strcmp(m[i].description, "VACCINS") == 0) revenues[3] += m[i].prix;
                    else if (strcmp(m[i].description, "ANTISEPTIQUES") == 0) revenues[4] += m[i].prix;
                    else if (strcmp(m[i].description, "DESINFECTANTS") == 0) revenues[5] += m[i].prix;
                }
                float maxRevenue = 0;
                for (int i = 0; i < 6; i++) {
                    if (maxRevenue < revenues[i]) {
                        maxRevenue = revenues[i];
                    }
                }
                for (int i = 0; i < 6; i++) {
                    if (revenues[i] == maxRevenue) {
                        switch (i) {
                            case 0: printf("Le type du medicament avec le meilleur revenu est : ORAUX\n"); break;
                            case 1: printf("Le type du medicament avec le meilleur revenu est : INJECTABLES\n"); break;
                            case 2: printf("Le type du medicament avec le meilleur revenu est : PERFUSION\n"); break;
                            case 3: printf("Le type du medicament avec le meilleur revenu est : VACCINS\n"); break;
                            case 4: printf("Le type du medicament avec le meilleur revenu est : ANTISEPTIQUES\n"); break;
                            case 5: printf("Le type du medicament avec le meilleur revenu est : DESINFECTANTS\n"); break;
                        }
                    }
                }
                break;
            }
            default:
                printf("Choix invalide. Veuillez reessayer.\n");
                break;
        }
        printf("Entrez votre choix (1, 2, 3, ou 4 pour quitter): ");
        scanf("%d", &choix);
    }
}

TypeMedoc StringToTypeMedoc(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }

    for (int i = 0; i < TYPE_COUNT; ++i) {
        if (strcmp(str, TypeMedocStrings[i]) == 0) {
            return (TypeMedoc)i;
        }
    }
    return TYPE_COUNT;
}
