#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <mysql/mysql.h>

int get_user_type(char *username)
{
    // Requête SQL pour récupérer le TypeUtilisateur en fonction de l'ID_Utilisateur
    char query[255];
    sprintf(query, "SELECT TypeUtilisateur FROM Utilisateur WHERE ID_Utilisateur = '%s'", username);

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
