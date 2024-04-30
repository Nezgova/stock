#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include<stdbool.h>
#include "../header/USER.h"
#include "../header/MEDICAMENT.h"
#include "../header/STOCK.h"

int main()
{
    Stock s;
    int num=50;
    User* user = malloc(num* sizeof(User));

    int choix;
    int nbMedicaments = 0;
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
        printf("7. Quitter\n");
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
            if (s.Medicaments)
            {
                free(s.Medicaments);
            }
            printf("Fermeture...\n");
            break;
        default:
            printf("Choix invalide ! Veuillez réessayer.\n");
        }
    } while (choix != 7);

    if (s.Medicaments)
    {
        free(s.Medicaments);
    }

    printf("Fermeture...\n");
    free(user);
    return 0;
}
