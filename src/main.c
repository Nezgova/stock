#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<stdbool.h>
#include "../header/USER.h"
#include "../header/MEDICAMENT.h"
#include "../header/STOCK.h"

void generateTestDataToFile(const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    fprintf(f, "Nom,Description,Prix,Quantite,Seuil d'alerte,Date d'entree,Date de sortie,Type Medicament\n");

    for (int i = 0; i < 1000; i++) {
        fprintf(f, "Medicament%d,Description%d,%.2f,%d,%d,%02d/%02d/%04d,%02d/%02d/%04d,%s\n",
                i + 1, i + 1, (float)(i + 1) * 10, i + 100, i + 10, (i % 28) + 1, (i % 12) + 1, 2022, (i % 28) + 1, (i % 12) + 1, 2022, TypeMedocToString(i % TYPE_COUNT));
    }

    fclose(f);
}

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        generateTestDataToFile("information/stock.csv");
        exit(0);
    }
    
    
    Stock s;
    int num=50;
    User* user = malloc(num* sizeof(User));

    int choix;
    int nbMedicaments = countLinesInFILE("information/stock.csv");
    s.Medicaments = NULL;

    login_singin_menu(user,&num);
    strcpy(s.user, user->username);
    //second menu
    do
    {
        printf("\nMenu Principal:\n");
        printf("1. Ajouter un Medicament\n");
        printf("2. Afficher le bilan\n");
        printf("3. Rechercher un Medicament\n");
        printf("4. Modifier un Medicament\n");
        printf("5. Supprimer un Medicament\n");
        printf("6. Disponibilité un Medicament\n");
        printf("7. Achat\n");
        printf("8. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);

        switch (choix)
        {
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
            supprimerDernierMedicament(&s, &nbMedicaments);
            printf("medicament supprimé avec succès.\n");
            break;
        case 6:
            chargerDepuisFichier(&s,&nbMedicaments);
            recherche_de_disponibilite(&s,nbMedicaments);
            break;
            case 7:
             chargerDepuisFichier(&s,&nbMedicaments);
            achat(s.Medicaments,nbMedicaments);
            break;
        case 8:
            if (s.Medicaments)
            {
                free(s.Medicaments);
            }
            printf("Fermeture...\n");
            break;
        default:
            printf("Choix invalide ! Veuillez réessayer.\n");
        }
    } while (choix != 8);

    if (s.Medicaments)
    {
        free(s.Medicaments);
    }

    printf("Fermeture...\n");
    free(user);
    return 0;
}
 