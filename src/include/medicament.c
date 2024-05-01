#include "../../header/MEDICAMENT.h"
#include <stdio.h>

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