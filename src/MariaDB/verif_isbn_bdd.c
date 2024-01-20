#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#define TAILLE_REQUETE 256

typedef struct {
    char isbn[14];
    char dateEmprunt[20];
    char siteDeRestitution[50];
} EmpruntInfo;

int trouverRetard(MYSQL *conn, const char *isbn, double *joursDeRetard) {
    MYSQL_RES *res;
    MYSQL_ROW row;
    int livreTrouve = 0;

    // Construire la requête SQL pour obtenir les informations sur les emprunts
    char query[TAILLE_REQUETE];
    sprintf(query, "SELECT e.ISBN, e.DateEmprunt, e.SiteDeRestitution FROM Emprunt e JOIN Exemplaire ex ON e.ID_Exemplaire = ex.ID_Exemplaire WHERE ex.ISBN = '%s'", isbn);

    // Exécuter la requête SQL
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "mysql_query() a échoué\n");
        return -1;  // Erreur
    }

    // Récupérer le résultat de la requête
    res = mysql_store_result(conn);

    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() a échoué\n");
        return -1;  // Erreur
    }

    EmpruntInfo dernierEmprunt;
    *joursDeRetard = 0;

    // Parcourir les résultats pour obtenir la date d'emprunt la plus récente
    while ((row = mysql_fetch_row(res)) != NULL) {
        livreTrouve = 1;
        sscanf(row[0], "%s", dernierEmprunt.isbn);
        sscanf(row[1], "%s", dernierEmprunt.dateEmprunt);
        sscanf(row[2], "%s", dernierEmprunt.siteDeRestitution);

        // Calculer le retard en jours
        // Le format de la date d'emprunt est par defaut dans la db : "YYYY-MM-DD HH:MM:SS"
        struct tm tm_emprunt;
        strptime(dernierEmprunt.dateEmprunt, "%Y-%m-%d %H:%M:%S", &tm_emprunt);
        time_t temps_emprunt = mktime(&tm_emprunt);

        time_t temps_actuel;
        time(&temps_actuel);

        *joursDeRetard = difftime(temps_actuel, temps_emprunt) / (60 * 60 * 24);
    }

    // Libérer les ressources
    mysql_free_result(res);

    return livreTrouve;
}