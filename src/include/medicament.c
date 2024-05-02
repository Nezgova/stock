#include "../../header/MEDICAMENT.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
void remplirMedicament(medicament *p)
{
    printf("Entrer le Nom du medicament : ");
    scanf("%49s", p->nom);
    rewind(stdin);

    printf("Entrer la Description du medicament : ");
    scanf("%99s", p->description);
    rewind(stdin);

    do {
        printf("Entrer le Prix du medicament : ");
    } while (scanf("%f", &p->prix) != 1 || p->prix <= 0); 

    do {
        printf("Entrer la Quantité du medicament en stock : ");
    } while (scanf("%d", &p->qnt) != 1 || p->qnt < 0); 

    do {
        printf("Entrer le Seuil d'alerte du medicament : ");
    } while (scanf("%d", &p->seuil_alrt) != 1 || p->seuil_alrt < 0); 

    do {
        printf("Entrer la Date de dernière entrée en stock du medicament (jour mois année) : ");
    } while (scanf("%hd%hd%hd", &p->entre.j, &p->entre.m, &p->entre.a) != 3); 

    do {
        printf("Entrer la Date de dernière sortie en stock du medicament (jour mois année) : ");
    } while (scanf("%hd%hd%hd", &p->sortie.j, &p->sortie.m, &p->sortie.a) != 3); 
}
void achat(medicament *m, int n)
{
    FILE *f = fopen("information/ticket.csv", "w");

    int i, choix = 1;
    char nom_med[20];
    int qt_med = 0;
    float total = 0;
    
    if (f == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(EXIT_FAILURE);
    }
    fprintf(f, "Nom du Medicament,Quantite,Prix\n");
    while (choix != 0)
    {
        printf("Entrer :\n1.Pour Acheter.\n2.Pour Quitter.\n");
    scanf("%d", &choix);
        printf("Entrer le nom du medicament :");
        scanf("%s", nom_med);
        printf("Entrer la quantite de votre medicament:");
        scanf("%d", &qt_med);
        for (i = 0; i < n; i++)
        {
            if (strcmp(nom_med, m[i].nom) == 0)
            {
                if (qt_med > m[i].qnt)
                {
                    printf("quantite insuffisante au stock\n");
                }
                else
                {
                    m[i].qnt -= qt_med;
                }
                if (m[i].qnt <= m[i].seuil_alrt)
                {
                    printf("Veuillez recharger le stock , il reste %d medicament nomee %s\n", m[i].qnt, m[i].nom);
                }
                printf("ACHAT REUSSI");
                break;
            }
        }

        fprintf(f, "%s,%d,%.2f\n", nom_med, qt_med, qt_med * m[i].prix);
        total += qt_med * m[i].prix;
    }
    fprintf(f, "TOTAL,%.2f", total);
    fclose(f);
}
