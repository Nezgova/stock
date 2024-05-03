#include "../../header/MEDICAMENT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

const char* TypeMedocStrings[] = {
    TYPE_MEDOC_LIST(STRING_DEF)
};

void remplirMedicament(medicament *p)
{
    printf("Entrer le Nom du medicament : ");
    scanf("%49s", p->nom);
    rewind(stdin);

    printf("Entrer la Description du medicament : ");
    scanf("%99s", p->description);
    rewind(stdin);

    do
    {
        printf("Entrer le Prix du medicament : ");
        rewind(stdin);
    } while (scanf("%f", &p->prix) != 1 || p->prix <= 0);

    do
    {
        printf("Entrer la Quantité du medicament en stock : ");
        rewind(stdin);
    } while (scanf("%d", &p->qnt) != 1 || p->qnt < 0);

    do
    {
        printf("Entrer le Seuil d'alerte du medicament : ");
        rewind(stdin);
    } while (scanf("%d", &p->seuil_alrt) != 1 || p->seuil_alrt < 0);

    do
    {
        printf("Entrer la Date de dernière entrée en stock du medicament (jour mois année) : ");
        rewind(stdin);
    } while (scanf("%hd%hd%hd", &p->entre.j, &p->entre.m, &p->entre.a) != 3);

    do
    {
        printf("Entrer la Date de dernière sortie en stock du medicament (jour mois année) : ");
        rewind(stdin);
    } while (scanf("%hd%hd%hd", &p->sortie.j, &p->sortie.m, &p->sortie.a) != 3);

    printf("Voulez-vous entrer le type du medicament comme une chaine de caractères ? (Oui/Non) : ");
    char response[4];
    scanf("%3s", response);
    rewind(stdin);

    bool useString = (strcmp(response, "Oui") == 0 || strcmp(response, "oui") == 0);

    for (int i = 0; i < TYPE_COUNT; ++i)
    {
        printf("%d. %s\n", i, TypeMedocStrings[i]);
    }

    if (useString)
    {
        char typeStr[20];
        do
        {
            printf("Entrer le Type du medicament : ");
            scanf("%19s", typeStr);
            rewind(stdin);
            p->Type = StringToTypeMedoc(typeStr);
        } while (p->Type == TYPE_COUNT);
    }
    else
    {
        int typeNum;
        do
        {
            printf("Entrer le Type du medicament (0-5) : ");
            scanf("%d", &typeNum);
            rewind(stdin);
            p->Type = (TypeMedoc)typeNum;
        } while (typeNum < 0 || typeNum >= TYPE_COUNT);
    }
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
    fprintf(f, "Nom du Medicament,Quantite,Prix,type\n");
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

        fprintf(f, "%s,%d,%.2f,%s\n", nom_med, qt_med, qt_med * m[i].prix, TypeMedocToString(m[i].Type));
        total += qt_med * m[i].prix;
    }
    fprintf(f, "TOTAL,%.2f", total);
    fclose(f);
}

TypeMedoc StringToTypeMedoc(char *str)
{

    for (int i = 0; str[i]; i++)
    {
        str[i] = toupper(str[i]);
    }

    for (int i = 0; i < TYPE_COUNT; ++i)
    {
        if (strcmp(str, TypeMedocStrings[i]) == 0)
        {
            return (TypeMedoc)i;
        }
    }
    return TYPE_COUNT;
}
