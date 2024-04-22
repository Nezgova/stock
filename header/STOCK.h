#ifndef STOCK_H
#define STOCK_H

#include <stdio.h>

typedef struct {
    int id;
    char name[50];
    float price;
    int quantity;
} Item;

#define CHECK_NULL(pointer) \
    do { \
        if (!(pointer)) { \
            fprintf(stderr, "Erreur: pointeur NULL à %s:%d\n", __FILE__, __LINE__); \
            return; \
        } \
    } while (0)

void read_stock_from_csv(const char *filename);
void write_stock_to_csv(const char *filename);
void add_item(Item *items, int *num_items);
void remove_item(Item *items, int *num_items, int item_id);
void update_item_quantity(Item *items, int num_items, int item_id, int quantity);

#endif