#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../header/ACHAT.h"

void achat(medicament *m, int n)
{
    int i;
    char nom_med[20];
    int qt_med = 0;
    printf("Entrer le nom du medicament :");
    scanf("%s", nom_med);
    printf("Entrer la quantite de votre medicament:");
    scanf("%d", &qt_med);
    for (i = 0; i < n; i++)
    {
        if (stcmp(nom_med, m[i].nom) == 0)
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
                printf("Veuillez recharger le stock , il reste %d medicament nome %s\n", m[i].qnt, m[i].nom);
            }
            printf("ACHAT REUSSI");
        }
    }
}
