#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <time.h>


// Fonction pour marquer les livres en transit comme "ValiderApresTransit" tous les mercredis
void valider_restitution_livres_en_transit(MYSQL *conn)
{
    // Obtenez la date actuelle
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    // Vérifiez si aujourd'hui est un mercredi (tm_wday=3 pour mercredi)
    if (tm_info->tm_wday == 3)
    {
        // Requête SQL pour marquer les livres en transit comme "ValiderApresTransit"
        char query[1024];
        sprintf(query, "UPDATE Restitution SET ValiderApresTransit=true WHERE EnTransit=true AND ValiderApresTransit=false");

        // Exécuter la requête SQL
        if (mysql_query(conn, query))
        {
            fprintf(stderr, "Erreur lors du marquage des livres en transit : %s\n", mysql_error(conn));
        }
    }
}

// faire une fonction qui permet à l'admin de valider la restitution