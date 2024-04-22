#ifndef USER_H
#define USER_H

#include <stdbool.h>
#include "STOCK.h"

typedef enum {
    ADMIN,
    BUYER
} Ranking;


typedef struct {
    char username[50];
    char password[50];
} User;

User *authenticate_user(const char *username, const char *password, User *users, int num_users);
bool register_user(const char *username, const char *password, User *users, int *num_users);
bool change_password(User *user, const char *old_password, const char *new_password);
bool delete_user(User *user, User *users, int *num_users);
bool save_users(const char *filename, User *users, int num_users);
bool load_users(const char *filename, User *users, int *num_users);
void login_singin_menu(User *users, int *num_users);

#endif