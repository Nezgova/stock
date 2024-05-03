#ifndef MEDICAMENT_H
#define MEDICAMENT_H

typedef struct
{
    short j, m, a;
} Date;

#define TYPE_MEDOC_LIST(X) \
    X(ORAUX) \
    X(INJECTABLES) \
    X(PERFUSION) \
    X(VACCINS) \
    X(ANTISEPTIQUES) \
    X(DESINFECTANTS)

#define ENUM_DEF(name) name,
#define STRING_DEF(name) #name,

typedef enum {
    
    TYPE_MEDOC_LIST(ENUM_DEF)
    TYPE_COUNT

} TypeMedoc;

extern const char* TypeMedocStrings[];

#define TypeMedocToString(type) \
    ((type >= 0 && type < TYPE_COUNT) ? TypeMedocStrings[type] : "UNKNOWN")
typedef struct
{
    int qnt, seuil_alrt;
    char id[11], nom[50], description[100];
    float prix;
    TypeMedoc Type;
    Date entre, sortie;
} medicament;

TypeMedoc StringToTypeMedoc(char* str);
void remplirMedicament(medicament *p);
void achat(medicament *m, int n);

#endif