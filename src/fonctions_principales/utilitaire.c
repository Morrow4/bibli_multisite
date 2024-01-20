#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"

int get_user_type(MYSQL *conn, char *username)
{
    // Requête SQL pour récupérer le TypeUtilisateur en fonction de l'Email
    char query[255];
    sprintf(query, "SELECT TypeUtilisateur FROM Utilisateur WHERE Email = '%s'", username);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(1);
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        mysql_close(conn);
        exit(1);
    }

    // Récupération du résultat
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row)
    {
        if (row[0] == "AdminGeneral")
        {
            user_group = 1;
        }
        else if (row[0] == "AdminSite")
        {
            user_group = 2;
        }
        else if (row[0] == "Inscrit")
        {
            user_group = 3;
        }
        else if (row[0] == "Invite")
        {
            user_group = 4;
        }
        else
        {
            printf("non trouve");
        }
        printf("Le type d'utilisateur pour %s est : %s\n", username, row[0]);
    }
    else
    {
        printf("Utilisateur non trouvé : %s\n", username);
    }

    mysql_free_result(result);
    mysql_close(conn);

    return (user_group);
}

bool estEntier(const char *str)
{
    char *c;

    if (!str || *str == '\0')
    {
        return false;
    }

    c = str;

    while (*c != 0)
    {
        if (*c < '0' || *c > '9')
        {
            return false;
        }
        c++;
    }
    return true;
}

bool limiteTailleInt(const char *str, int limite)
{
    if (!str || *str == '\0')
    {
        return false;
    }

    if (strlen(str) > limite)
    {
        return false;
    }

    return estEntier(str);
}

bool gestion_int(int valeur)
{
    char str[11];                             // une taille suffisamment grande pour stocker un int
    snprintf(str, sizeof(str), "%d", valeur); // convertit l'entier en chaîne de caractères

    if (!estEntier(str))
    {
        printf("La valeur n'est pas un entier.\n");
        return false;
    }

    if (!limiteTailleInt(str, 9))
    {
        printf("Nombre incorrect.\n");
        return false;
    }

    return true;
}

void qui_et_quand(char **username, char **time_str)
{
    // Obtenir le nom de l'utilisateur à l'origine de l'exécution
    qui(username);

    // Obtenir la date et l'heure actuelles
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    *time_str = malloc(64);
    strftime(*time_str, 64, "%Y-%m-%d %H:%M:%S", local_time);
}

void qui(char **username)
{
    // Obtenir le nom de l'utilisateur à l'origine de l'exécution
    uid_t uid = getuid();
    struct passwd *pwd = getuid(uid);
    *username = (pwd != NULL) ? pwd->pw_name : "Inconnu";
}