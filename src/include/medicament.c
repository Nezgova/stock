#include "../../header/MEDICAMENT.h"
#include <stdio.h>

void remplirMedicament(medicament *p)
{
    printf("Entrer le Nom du medicament : ");
    scanf("%49s", p->nom);
    printf("Entrer la Description du medicament : ");
    scanf("%99s", p->description);
    printf("Entrer le Prix du medicament : ");
    scanf("%f", &p->prix);
    printf("Entrer la Quantité du medicament en stock : ");
    scanf("%d", &p->qnt);
    printf("Entrer le Seuil d'alerte du medicament : ");
    scanf("%d", &p->seuil_alrt);
    printf("Entrer la Date de dernière entrée en stock du medicament (jour mois année) : ");
    scanf("%hd%hd%hd", &p->entre.j, &p->entre.m, &p->entre.a);
    printf("Entrer la Date de dernière sortie en stock du medicament (jour mois année) : ");
    scanf("%hd%hd%hd", &p->sortie.j, &p->sortie.m, &p->sortie.a);
}

