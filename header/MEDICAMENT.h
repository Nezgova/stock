#ifndef MEDICAMENT_H
#define MEDICAMENT_H

typedef struct
{
    short j, m, a;
} Date;

typedef enum {
    ORAUX,
    INJECTABLES,
    PERFUSION,
    VACCINS,
    ANTISEPTIQUES,
    DESINFECTANTS
} TypeMedoc;

typedef struct
{
    int qnt, seuil_alrt;
    char id[11], nom[50], description[100];
    float prix;
    TypeMedoc Type;
    Date entre, sortie;
} medicament;

void remplirMedicament(medicament *p);
void achat(medicament *m, int n);

#endif