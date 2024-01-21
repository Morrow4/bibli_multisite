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

// utilitaire.c
int get_user_group(MYSQL *conn);
bool estEntier(const char *str);
bool limiteTailleInt(const char *str, int limite);
bool gestion_int(int valeur);
void qui_et_quand(char **username, char **time_str);
char *qui();
void deconnexion(MYSQL *conn);

// MariaDB - connect.c
MYSQL *connect_database();

#endif
