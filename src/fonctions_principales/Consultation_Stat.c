// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

void Consultation_Stat() {
    return;
}

void AfficherStatsParSite(MYSQL *conn) {
    // Construction de la requête SQL pour récupérer le nombre d'emprunts et de réservations par site
    const char* query = "";

    // Exécution de la requête
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Erreur lors de la récupération des statistiques : %s\n", mysql_error(conn));
        exit(EXIT_FAILURE);
    }

    // Récupération du résultat
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed\n");
        exit(EXIT_FAILURE);
    }

    // Affichage des statistiques par site
    printf("%-20s%-15s%-15s\n", "Site", "Emprunts", "Réservations");
    printf("--------------------------------------------\n");

    // Parcours des lignes du résultat
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%-20s%-15s%-15s\n", row[0], row[1], row[2]);
    }

    // Libération de la mémoire du résultat
    mysql_free_result(result);
}