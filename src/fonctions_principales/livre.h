#ifndef LIVRE_H
#define LIVRE_H

#include <mysql/mysql.h>

#include <stdio.h>
#include <stdlib.h>

void ajout_livre(MYSQL *conn);
void mise_a_jour_livre(MYSQL *conn, char *ISBN);
void suppression_livre(MYSQL *conn, char *ISBN);

#endif