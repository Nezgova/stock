#ifndef ACHAT_H
#define ACHAT_H
#include <stdio.h>
#include<string.h>

#include "MEDICAMENT.h"

typedef struct
{
    char user[50];
    medicament *Medicaments;
} Stock;

void achat(medicament *m, int n);

#endif