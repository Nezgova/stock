#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "../header/USER.h"
#include "../header/MEDICAMENT.h"
#include "../header/STOCK.h"

void generateTestDataToFile(const char *filename) {
    

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie,Type Medicament\n");

    srand(time(NULL));

    for (int i = 0; i < 1000; i++) {
        int prix = (rand() % 1000) + 1; 
        int quantite = (rand() % 100) + 1;
        int seuilAlerte = (rand() % 20) + 1;
        int jourEntree = (rand() % 28) + 1; 
        int moisEntree = (rand() % 12) + 1; 
        int anneeEntree = 2022; 
        int jourSortie = (rand() % 28) + 1; 
        int moisSortie = (rand() % 12) + 1; 
        int anneeSortie = 2022; 
        const char *typeMedicament =  TypeMedocToString(rand() % TYPE_COUNT);  

        fprintf(f, "Medicament%d,Description%d,%.2f,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d,%s\n",
                i + 1, i + 1, (float)prix, quantite, seuilAlerte, 
                jourEntree, moisEntree, anneeEntree, 
                jourSortie, moisSortie, anneeSortie, typeMedicament);
    }

    fclose(f);
}

void modifierMedicament(Stock *s, int n);

int main(int argc, char *argv[]) {
    if (argc == 2) {
        generateTestDataToFile("information/stock.csv");
        exit(0);
    }

    Stock s;
    User *users = malloc(50 * sizeof(User));
    int numUsers = 50;
    int nbMedicaments = countLinesInFILE("information/stock.csv") - 1; // Subtract 1 for the header line
    s.Medicaments = NULL;

    login_singin_menu(users, &numUsers);
    strcpy(s.user, users->username);
    chargerDepuisFichier(&s, &nbMedicaments);

    while (true) {
        printf("\nMenu Principal:\n");
        printf("1. Ajouter un Medicament\n");
        printf("2. Afficher le bilan\n");
        printf("3. Rechercher un Medicament\n");
        printf("4. Modifier un Medicament\n");
        printf("5. Supprimer un Medicament\n");
        printf("6. Disponibilite un Medicament\n");
        printf("7. Achat\n");
        printf("8. Historique d'achat\n");
        printf("9. Statistiques\n");
        printf("10. Afficher graphique des quantités\n");
        printf("11. Afficher graphique avancé des quantités\n");
        printf("12. Quitter\n");
        printf("Choix: ");
        int choix;
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouterStockMedicament(&s, &nbMedicaments);
                break;
            case 2:
                genererBilanCSV(&s, nbMedicaments);
                printf("Bilan généré avec succès.\n");
                break;
            case 3:
                rechercherMedicament(&s, nbMedicaments);
                break;
            case 4:
                modifierMedicament(&s, nbMedicaments);
                break;
            case 5:
                printf("Entrer la position de votre medicament:\n");
                int pos;
                scanf("%d", &pos);
                supprimerStockMedicament(&s, &nbMedicaments, pos);
                printf("Medicament supprimé avec succès.\n");
                break;
            case 6:
                recherche_de_disponibilite(&s, nbMedicaments);
                break;
            case 7:
                achat(s.Medicaments, nbMedicaments);
                break;
            case 8: {
                medicament *history = malloc(nbMedicaments * sizeof(medicament));
                chargerDepuisFichierHistory(&history, &nbMedicaments);
                affichage_history(history, nbMedicaments);
                free(history);
                break;
            }
            case 9: {
                medicament *history = malloc(nbMedicaments * sizeof(medicament));
                chargerDepuisFichierHistory(&history, &nbMedicaments);
                printf("Entrer :\n1. Le type de medicament le plus acheté.\n2. Le revenu total.\n3. Le type avec le meilleur revenu.\n4. Quitter.\n");
                int ch;
                scanf("%d", &ch);
                statistics(history, ch);
                free(history);
                break;
            }
            case 10:
                afficherGraphiqueQuantites(&s, nbMedicaments);
                break;
            case 11:
                afficherGraphiqueQuantitesAvance(&s, nbMedicaments);
                break;
            case 12:
                printf("Fermeture...\n");
                free(s.Medicaments);
                free(users);
                return 0;
            default:
                printf("Choix invalide ! Veuillez réessayer.\n");
        }
    }
}
