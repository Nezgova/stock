#ifndef STOCK_H
#define STOCK_H

#include <stdio.h>

#include "MEDICAMENT.h"

typedef struct
{
    char user[50];
    medicament *Medicaments;
} Stock;

int countLinesInFILE(const char *filename); 

void ajouterStockMedicament(Stock *s, int *n);
void supprimerDernierMedicament(Stock *s, int *n);
void modifierMedicament(Stock *s, int n);
void afficherMedicaments(Stock *s, int n);
void rechercherMedicament(Stock *s, int n);
void recherche_de_disponibilite(Stock *s, int n);
void ModificationStockMedicament(int n, int position,medicament m);
void genererBilanCSV(Stock *s, int nbMedicaments);
void chargerDepuisFichier(Stock *s, int *n);
#endif