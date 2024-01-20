#ifndef UTILITAIRE_H
#define UTILITAIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <mysql/mysql.h>
#include <mysql/mysql.h>

/* // Structure pour représenter un utilisateur
typedef struct
{
    int ID_Utilisateur;
    char Nom[255];
    char Prenom[255];
    char Email[255];
    char MotDePasse[255];
    char TypeUtilisateur[255];
    int EstChercheur;
    int EstBloque;
    char CommentaireBlocage[255];
} Utilisateur; */

// utilitaire.c
int get_user_type(MYSQL *conn, char *username);
bool estEntier(const char *str);
bool limiteTailleInt(const char *str, int limite);
bool gestion_int(int valeur);
void qui_et_quand(char **username, char **time_str);
void qui(char **username);

#endif
