// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

void consultation_stat(MYSQL *conn) {
    return;
}

void consultation_stat_site(MYSQL *conn) {
    // Saisie utilisateur pour choisir un site
    char site[20];
    printf("Choisissez un site (Site A, Site B, Site C) : ");
    fgets(site, sizeof(site), stdin);
    site[strcspn(site, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie

     // Saisie utilisateur pour choisir l'unité de temps
    char unite[10];
    printf("Choisissez l'unité de temps (jour, mois, année) : ");
    fgets(unite, sizeof(unite), stdin);
    unite[strcspn(unite, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie

    // Construction de la requête SQL pour récupérer le nombre d'emprunts et de réservations par site
    char query[1000];
    snprintf(query, sizeof(query),
             "SELECT Exemplaire.SitePrincipal, "
             "COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts, "
             "COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
             "FROM Emprunt "
             "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
             "LEFT JOIN Reservation ON Exemplaire.ID_Exemplaire = Reservation.ID_Exemplaire "
             "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d') "
             "AND DATE_FORMAT(Reservation.DateReservation, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d') "
             "AND Exemplaire.SitePrincipal = '%s' "
             "GROUP BY Exemplaire.SitePrincipal;", site);

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