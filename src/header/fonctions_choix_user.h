#ifndef FONCTIONS_CHOIX_USER
#define FONCTIONS_CHOIX_USER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>

void choix_invite_bibliotheque(MYSQL *conn);
void choix_inscrit_bibliotheque(MYSQL *conn, char *username);
void choix_admin_site_bibliotheque(MYSQL *conn, char *username);
void choix_admin_general_bibliotheque(MYSQL *conn, char *username);

#endif
