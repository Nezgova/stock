#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "../../header/USER.h"

User *authenticate_user(const char *username, const char *password, User *users, int num_users)
{
    for (int i = 0; i < num_users; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            return &users[i];
        }
    }
    return NULL;
}

bool register_user(const char *username, const char *password, User *users, int *num_users)
{
    if (*num_users >= 50)
    {
        return false;
    }
    if (username == NULL || password == NULL)
    {
        return false;
    }

    char username1[50];
    strcpy(username1, username);
    for (int i = 0; username1[i]; i++)
    {
        username1[i] = toupper(username1[i]);
    }

    for (int i = 0; i < *num_users; i++)
    {

        char username2[50];
        strcpy(username2, users[i].username);
        for (int j = 0; username2[j]; j++)
        {
            username2[j] = toupper(username2[j]);
        }

        if (strcmp(username2, username1) == 0)
        {
            return false;
        }
    }

    for (int i = 0; username[i] != '\0'; i++)
    {
        if (username[i] == ' ')
        {
            return false;
        }
    }
    for (int i = 0; password[i] != '\0'; i++)

    {
        if (password[i] == ' ')
        {
            return false;
        }
    }
    for (const char *ptr = username; *ptr != '\0'; ptr++)
    {
        if (!isalnum(*ptr))
        {
            return false;
        }
    }

    for (const char *ptr = password; *ptr != '\0'; ptr++)
    {
        if (!isprint(*ptr))
        {
            return false;
        }
    }

    strcpy(users[*num_users].username, username);
    strcpy(users[*num_users].password, password);

    (*num_users)++;
    return true;
}

bool change_password(User *user, const char *old_password, const char *new_password)
{
    if (strcmp(user->password, old_password) == 0)
    {
        strcpy(user->password, new_password);
        return true;
    }
    return false;
}

bool delete_user(User *user, User *users, int *num_users)
{
    for (int i = 0; i < *num_users; i++)
    {
        if (&users[i] == user)
        {
            for (int j = i; j < *num_users - 1; j++)
            {
                users[j] = users[j + 1];
            }
            (*num_users)--;
            return true;
        }
    }
    return false;
}

bool save_users(const char *filename, User *users, int num_users)
{
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        return false;
    }
    fwrite(users, sizeof(User), num_users, file);
    fclose(file);
    return true;
}

bool load_users(const char *filename, User *users, int *num_users)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        return false;
    }
    *num_users = fread(users, sizeof(User), 50, file);
    fclose(file);
    return true;
}
void login_singin_menu(User *users, int *num_users)
{
    char nom[50];
    char password[50];
    char new_pass[50];
    if (!load_users("auth/enc.txt", users, num_users)) {
        printf("erreur\n");
        return;
    }
    while (true)
    {
        printf("\n1. Se connecter\n");
        printf("2. S'inscrire\n");
        printf("3. Changer de mot de passe\n");
        printf("4. Quitter\n");
        printf("Entrez votre choix : ");

        int choix;
        scanf("%d", &choix);

        switch (choix)
        {
        case 1:
            rewind(stdin);
            printf("\nEntrez le nom d'utilisateur : ");
            scanf("%s", nom);
            printf("Entrez le mot de passe : ");
            scanf("%s", password);

            User *auth = authenticate_user(nom, password, users, *num_users);
            if (auth != NULL)
            {
                printf("Bienvenue, %s !\n", auth->username);
                return ;
            }
            else
            {
                printf("Authentification échouée. Nom d'utilisateur ou mot de passe invalide.\n");
            }
            break;
        case 2:
            do
            {
                printf("\nEntrez le nouveau nom d'utilisateur : ");
                scanf("%s", nom);
                printf("Entrez le nouveau mot de passe : ");
                scanf("%s", password);

                if (register_user(nom, password, users, num_users))
                {
                    printf("Inscription réussie !\n");

                    if (!save_users("auth/enc.txt", users, *num_users))
                    {
                        printf("Erreur lors de l'enregistrement.\n");
                    }
                    break;
                }
                else
                {
                    printf("Inscription échouée. Nom d'utilisateur déjà pris ou nombre maximal d'utilisateurs atteint.\n");
                }
            } while (true);
            break;
        case 3:
            printf("\nEntrez le nom d'utilisateur : ");
            scanf("%s", nom);
            printf("Entrez l'ancien mot de passe : ");
            scanf("%s", password);
            printf("Entrez le nouveau mot de passe : ");
            scanf("%s", new_pass);

            User *user_new_pass = authenticate_user(nom, password, users, *num_users);
            if (user_new_pass != NULL)
            {
                if (change_password(user_new_pass, password, new_pass))
                {
                    printf("Mot de passe modifié avec succès !\n");
                    return ;
                }
                else
                {
                    printf("Impossible de modifier le mot de passe. Ancien mot de passe invalide.\n");
                }
            }
            else
            {
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