#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../header/USER.h"

#define MAX_USERS 50
#define USERNAME_LENGTH 50
#define PASSWORD_LENGTH 50

User *authenticate_user(const char *username, const char *password, User *users, int num_users) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0) {
            return &users[i];
        }
    }
    return NULL;
}

bool register_user(const char *username, const char *password, User *users, int *num_users) {
    if (*num_users >= MAX_USERS) {
        printf("Le nombre maximal d'utilisateurs est atteint.\n");
        return false;
    }
    if (username == NULL || password == NULL) {
        printf("Nom d'utilisateur ou mot de passe invalide.\n");
        return false;
    }

    char username_upper[USERNAME_LENGTH];
    strcpy(username_upper, username);
    for (int i = 0; username_upper[i]; i++) {
        username_upper[i] = toupper(username_upper[i]);
    }

    for (int i = 0; i < *num_users; i++) {
        char username_existing[USERNAME_LENGTH];
        strcpy(username_existing, users[i].username);
        for (int j = 0; username_existing[j]; j++) {
            username_existing[j] = toupper(username_existing[j]);
        }

        if (strcmp(username_existing, username_upper) == 0) {
            printf("Nom d'utilisateur déjà pris.\n");
            return false;
        }
    }

    for (int i = 0; username[i] != '\0'; i++) {
        if (username[i] == ' ' || !isalnum(username[i])) {
            printf("Nom d'utilisateur invalide. Utilisez uniquement des lettres et des chiffres sans espaces.\n");
            return false;
        }
    }

    for (int i = 0; password[i] != '\0'; i++) {
        if (password[i] == ' ' || !isprint(password[i])) {
            printf("Mot de passe invalide. Utilisez uniquement des caractères imprimables sans espaces.\n");
            return false;
        }
    }

    strcpy(users[*num_users].username, username);
    strcpy(users[*num_users].password, password);
    (*num_users)++;
    return true;
}

bool change_password(User *user, const char *old_password, const char *new_password) {
    if (strcmp(user->password, old_password) == 0) {
        strcpy(user->password, new_password);
        return true;
    }
    return false;
}

bool delete_user(User *user, User *users, int *num_users) {
    for (int i = 0; i < *num_users; i++) {
        if (&users[i] == user) {
            for (int j = i; j < *num_users - 1; j++) {
                users[j] = users[j + 1];
            }
            (*num_users)--;
            return true;
        }
    }
    return false;
}

bool save_users(const char *filename, User *users, int num_users) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier pour sauvegarde.\n");
        return false;
    }
    fwrite(users, sizeof(User), num_users, file);
    fclose(file);
    return true;
}

bool load_users(const char *filename, User *users, int *num_users) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return false;
    }
    *num_users = fread(users, sizeof(User), MAX_USERS, file);
    fclose(file);
    return true;
}

void login_singin_menu(User *users, int *num_users) {
    char nom[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
    char new_pass[PASSWORD_LENGTH];
    if (!load_users("auth/enc.txt", users, num_users)) {
        printf("Erreur lors du chargement des utilisateurs.\n");
        return;
    }
    while (true) {
        printf("\n1. Se connecter\n");
        printf("2. S'inscrire\n");
        printf("3. Changer de mot de passe\n");
        printf("4. Quitter\n");
        printf("Entrez votre choix : ");

        int choix;
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("\nEntrez le nom d'utilisateur : ");
                scanf("%s", nom);
                printf("Entrez le mot de passe : ");
                scanf("%s", password);

                User *auth = authenticate_user(nom, password, users, *num_users);
                if (auth != NULL) {
                    printf("Bienvenue, %s !\n", auth->username);
                    return;
                } else {
                    printf("Authentification échouée. Nom d'utilisateur ou mot de passe invalide.\n");
                }
                break;
            case 2:
                printf("\nEntrez le nouveau nom d'utilisateur : ");
                scanf("%s", nom);
                printf("Entrez le nouveau mot de passe : ");
                scanf("%s", password);

                if (register_user(nom, password, users, num_users)) {
                    printf("Inscription réussie !\n");
                    if (!save_users("auth/enc.txt", users, *num_users)) {
                        printf("Erreur lors de l'enregistrement des utilisateurs.\n");
                    }
                } else {
                    printf("Inscription échouée.\n");
                }
                break;
            case 3:
                printf("\nEntrez le nom d'utilisateur : ");
                scanf("%s", nom);
                printf("Entrez l'ancien mot de passe : ");
                scanf("%s", password);
                printf("Entrez le nouveau mot de passe : ");
                scanf("%s", new_pass);

                User *user_new_pass = authenticate_user(nom, password, users, *num_users);
                if (user_new_pass != NULL) {
                    if (change_password(user_new_pass, password, new_pass)) {
                        printf("Mot de passe modifié avec succès !\n");
                    } else {
                        printf("Impossible de modifier le mot de passe. Ancien mot de passe invalide.\n");
                    }
                } else {
                    printf("Authentification échouée. Nom d'utilisateur ou mot de passe invalide.\n");
                }
                break;
            case 4:
                printf("Quitter...\n");
                return;
            default:
                printf("Choix invalide. Veuillez entrer un nombre entre 1 et 4.\n");
        }
    }
}
