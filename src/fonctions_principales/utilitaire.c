#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>
#include <time.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"

char *qui()
{
    // Obtenir le nom de l'utilisateur à l'origine de l'exécution
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    return (pwd != NULL) ? strdup(pwd->pw_name) : NULL;
}

int get_user_group(MYSQL *conn)
{
    char *username = qui();

    if (username == NULL)
    {
        fprintf(stderr, "Erreur lors de la récupération du nom d'utilisateur.\n");
        return -1; // Une valeur spéciale pour indiquer une erreur
    }

    int user_group = 0;

    // Requête SQL pour récupérer le TypeUtilisateur en fonction de l'Email
    char query[255];
    snprintf(query, sizeof(query), "SELECT TypeUtilisateur FROM Utilisateur WHERE Email = '%s'", username);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_error(conn));
        return -1; // Une valeur spéciale pour indiquer une erreur
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return -1; // Une valeur spéciale pour indiquer une erreur
    }

    // Récupération du résultat
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row)
    {
        if (strcmp(row[0], "AdminGeneral") == 0)
        {
            user_group = 1;
        }
        else if (strcmp(row[0], "AdminSite") == 0)
        {
            user_group = 2;
        }
        else if (strcmp(row[0], "Inscrit") == 0)
        {
            user_group = 3;
        }
        else if (strcmp(row[0], "Invite") == 0)
        {
            user_group = 4;
        }
        else
        {
            printf("Non trouvé\n");
        }
        printf("Le type d'utilisateur pour %s est : %s\n", username, row[0]);
    }
    else
    {
        printf("Utilisateur non trouvé : %s\n", username);
    }

    mysql_free_result(result);
    free(username); // Assurez-vous de libérer la mémoire allouée par strdup

    return user_group;
}

bool estEntier(const char *str)
{
    const char *c;

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

    if (strlen(str) > (size_t)limite) //comparaison en unsigned long
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
    qui(*username);

    // Obtenir la date et l'heure actuelles
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    *time_str = malloc(64);
    strftime(*time_str, 64, "%Y-%m-%d %H:%M:%S", local_time);
}

// Fonction pour déconnecter l'utilisateur
void deconnexion(MYSQL *conn)
{
    // Fermer la connexion à la base de données si elle est ouverte
    if (conn != NULL)
    {
        mysql_close(conn);
        printf("Déconnexion de la base de données réussie.\n");
    }

    // Terminer le programme
    exit(EXIT_SUCCESS);
}
