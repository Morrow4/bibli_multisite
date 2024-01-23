// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>

void consultation_stat_site(MYSQL *conn) {
    // Saisie utilisateur pour choisir un site
    char site[20];
    while (strcmp(site, "jour") != 0 && strcmp(site, "mois") != 0 && strcmp(site, "année") != 0) {
        printf("Choisissez un site (Site A, Site B, Site C) : ");
        fgets(site, sizeof(site), stdin);
        site[strcspn(site, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie
    }

    // Saisie utilisateur pour choisir l'unité de temps
    char unite[10];
    while (strcmp(unite, "jour") != 0 && strcmp(unite, "mois") != 0 && strcmp(unite, "année") != 0) {
        printf("Choisissez l'unité de temps (jour, mois, année) : ");
        fgets(unite, sizeof(unite), stdin);
        unite[strcspn(unite, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie
    }

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
    }

    // Récupération du résultat
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed\n");
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

void consultation_stat_3site(MYSQL* conn) {
    // Saisie utilisateur pour choisir l'unité de temps
    char unite[10];
    while (strcmp(unite, "jour") != 0 && strcmp(unite, "mois") != 0 && strcmp(unite, "année") != 0) {
        printf("Choisissez l'unité de temps (jour, mois, année) : ");
        fgets(unite, sizeof(unite), stdin);
        unite[strcspn(unite, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie
    }

    // Construction de la requête SQL pour récupérer le nombre d'emprunts et de réservations par site
    char query[1000];
    snprintf(query, sizeof(query),
            "SELECT COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts, "
            "COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
            "FROM Emprunt "
            "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
            "LEFT JOIN Reservation ON Exemplaire.ID_Exemplaire = Reservation.ID_Exemplaire "
            "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d') "
            "AND DATE_FORMAT(Reservation.DateReservation, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d');");


    // Exécution de la requête
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Erreur lors de la récupération des statistiques : %s\n", mysql_error(conn));
    }

    // Récupération du résultat
    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed\n");
    }

    // Affichage des statistiques
    printf("%-15s%-15s\n", "Emprunts", "Réservations");
    printf("---------------------------\n");

    // Parcours des lignes du résultat
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%-15s%-15s\n", row[0], row[1]);
    }

    // Libération de la mémoire du résultat
    mysql_free_result(result);
}

void consultation_stat(MYSQL *conn) {
    int user_type = get_user_group(conn);
    switch (user_type)
    {
    case 1: // admingeneral
        printf("Veuillez choisir de consulter les statistiques par site ou les 3 sites : 1 / 2");
        int choix;
        scanf("%d", &choix);
        switch (choix) 
        {
            case 1: // consultation stat pour les 3 sites
                consultation_stat_3site(conn);
            case 2: // consultation stat par site
                consultation_stat_site(conn);
            default: // choix invalide
                consultation_stat(conn);
        }
    case 2: // adminsite
        consultation_stat_site(conn);
    default: // autre
        consultation_stat(conn);
    }
}