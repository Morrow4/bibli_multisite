// Inclusion des en-têtes nécessaires_t
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"

void consultation_stat_site(MYSQL *conn, int user_type) {
    MYSQL_RES *res;
    MYSQL_ROW row;

    // Saisie utilisateur pour choisir un site
    char site[20];
    switch (user_type)
    {
        case 1: // admin general
            while (strcmp(site, "Site A") != 0 && strcmp(site, "Site B") != 0 && strcmp(site, "Site C") != 0) {
                while (getchar() != '\n');
                printf("Choisissez un site (Site A, Site B, Site C) : ");
                fgets(site, sizeof(site), stdin);
                site[strcspn(site, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie
                system("clear");
            }
            continue;
        case 2: // admin site
            if (mysql_query(conn, "SELECT SitePrincipal FROM Utilisateur") != 0) {
            fprintf(stderr, "Erreur lors de la récupération des statistiques : %s\n", mysql_error(conn));
            consultation_stat(conn, user_type);
            }
            // Récupère le résultat de la requête
            res = mysql_store_result(conn);
            // Vérifie s'il y a un résultat
            if (res) {
                // Récupère la première ligne du résultat
                row = mysql_fetch_row(res);

                // Vérifie s'il y a une colonne dans la ligne
                if (row) {
                    // Affiche la valeur de la colonne SitePrincipal
                    printf("Site Principal : %s\n", row[0]);
                    strcpy(site, row[0]);
                } else {
                    printf("Aucun résultat trouvé.\n");
                    consultation_stat(conn, user_type);
                }
            } else {
                fprintf(stderr, "Erreur lors de la récupération du résultat : %s\n", mysql_error(conn));
                consultation_stat(conn, user_type);
            }
            break;
        default:
            consultation_stat_site(conn, user_type);
            printf("Entrée erronée");
            break;
    }

    // Saisie utilisateur pour choisir l'unité de temps
    char unite[20];
    while (strcmp(unite, "jour") != 0 && strcmp(unite, "mois") != 0 && strcmp(unite, "année") != 0) {
        while (getchar() != '\n');
        printf("Choisissez l'unité de temps (jour, mois, année) : ");
        fgets(unite, sizeof(unite), stdin);
        unite[strcspn(unite, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie
        system("clear");
    }

    // Construisez la requête en fonction de l'unité de temps choisie pour les emprunts
    char query[1000];
    if (strcmp(unite, "jour") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT Exemplaire.SitePrincipal, "
                 "COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts "
                 "FROM Emprunt "
                 "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d') "
                 "AND Exemplaire.SitePrincipal = '%s' "
                 "GROUP BY Exemplaire.SitePrincipal;", site);
    } else if (strcmp(unite, "mois") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT Exemplaire.SitePrincipal, "
                 "COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts, "
                 "FROM Emprunt "
                 "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m') = DATE_FORMAT(NOW(), '%%Y-%%m') "
                 "AND Exemplaire.SitePrincipal = '%s' "
                 "GROUP BY Exemplaire.SitePrincipal;", site);
    } else if (strcmp(unite, "année") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT Exemplaire.SitePrincipal, "
                 "COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts, "
                 "FROM Emprunt "
                 "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y') = DATE_FORMAT(NOW(), '%%Y') "
                 "AND Exemplaire.SitePrincipal = '%s' "
                 "GROUP BY Exemplaire.SitePrincipal;", site);
    }

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
    printf("%-20s%-15s\n", "Site", "Emprunts");
    printf("--------------------------------------------\n");

    // Parcours des lignes du résultat
    //MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%-20s%-15s\n", row[0], row[1]);
        printf("\n");
    }

    // Construisez la requête en fonction de l'unité de temps choisie pour les réservations
    if (strcmp(unite, "jour") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT Exemplaire.SitePrincipal, "
                 "COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
                 "FROM Reservation "
                 "LEFT JOIN Exemplaire ON Reservation.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Reservation.DateReservation, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d') "
                 "AND Exemplaire.SitePrincipal = '%s' "
                 "GROUP BY Exemplaire.SitePrincipal;", site);
    } else if (strcmp(unite, "mois") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT Exemplaire.SitePrincipal, "
                 "COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
                 "FROM Reservation "
                 "LEFT JOIN Exemplaire ON Reservation.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m') = DATE_FORMAT(NOW(), '%%Y-%%m') "
                 "AND DATE_FORMAT(Reservation.DateReservation, '%%Y-%%m') = DATE_FORMAT(NOW(), '%%Y-%%m') "
                 "AND Exemplaire.SitePrincipal = '%s' "
                 "GROUP BY Exemplaire.SitePrincipal;", site);
    } else if (strcmp(unite, "année") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT Exemplaire.SitePrincipal, "
                 "COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
                 "FROM Reservation "
                 "LEFT JOIN Exemplaire ON Reservation.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Reservation.DateReservation, '%%Y') = DATE_FORMAT(NOW(), '%%Y') "
                 "AND Exemplaire.SitePrincipal = '%s' "
                 "GROUP BY Exemplaire.SitePrincipal;", site);
    }

    // Exécution de la requête
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Erreur lors de la récupération des statistiques : %s\n", mysql_error(conn));
    }
    // Récupération du résultat
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed\n");
    }

    // Affichage des statistiques par site
    printf("%-20s%-15s\n", "Site", "Réservations");
    printf("--------------------------------------------\n");

    // Parcours des lignes du résultat
    //MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%-20s%-15s\n", row[0], row[1]);
        printf("\n");
    }

    // Libération de la mémoire du résultat
    mysql_free_result(result);
}
   
void consultation_stat_3site(MYSQL* conn) {
    // Saisie utilisateur pour choisir l'unité de temps
    char unite[20];
    while (strcmp(unite, "jour") != 0 && strcmp(unite, "mois") != 0 && strcmp(unite, "année") != 0) {
        while (getchar() != '\n');
        printf("Choisissez l'unité de temps (jour, mois, année) : ");
        fgets(unite, sizeof(unite), stdin);
        unite[strcspn(unite, "\n")] = '\0'; // Supprimer le caractère de nouvelle ligne de la saisie
        system("clear");
    }
    // Construction de la requête SQL pour récupérer le nombre d'emprunts par site
    char query[1000];

    // Construisez la requête en fonction de l'unité de temps choisie
    if (strcmp(unite, "jour") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts "
                 "FROM Emprunt "
                 "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d');");
    } else if (strcmp(unite, "mois") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts "
                 "FROM Emprunt "
                 "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y-%%m') = DATE_FORMAT(NOW(), '%%Y-%%m');");
    } else if (strcmp(unite, "année") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT COUNT(DISTINCT Emprunt.ID_Emprunt) AS nb_emprunts "
                 "FROM Emprunt "
                 "LEFT JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire "
                 "WHERE DATE_FORMAT(Emprunt.DateEmprunt, '%%Y') = DATE_FORMAT(NOW(), '%%Y');");
    }

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
    printf("%-15s\n", "Emprunts");
    printf("---------------------------\n");

    // Parcours des lignes du résultat
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%-15s\n", row[0]);
        printf("\n");
    }

    // Construisez la requête en fonction de l'unité de temps choisie pour les réservations
    if (strcmp(unite, "jour") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
                 "FROM Reservation "
                 "WHERE DATE_FORMAT(Reservation.DateReservation, '%%Y-%%m-%%d') = DATE_FORMAT(NOW(), '%%Y-%%m-%%d');");
    } else if (strcmp(unite, "mois") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
                 "FROM Reservation "
                 "WHERE DATE_FORMAT(Reservation.DateReservation, '%%Y-%%m') = DATE_FORMAT(NOW(), '%%Y-%%m');");
    } else if (strcmp(unite, "année") == 0) {
        snprintf(query, sizeof(query),
                 "SELECT COUNT(DISTINCT Reservation.ID_Reservation) AS nb_reservations "
                 "FROM Reservation "
                 "WHERE DATE_FORMAT(Reservation.DateReservation, '%%Y') = DATE_FORMAT(NOW(), '%%Y');");
    }

    // Exécution de la requête
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Erreur lors de la récupération des statistiques : %s\n", mysql_error(conn));
    }

    // Récupération du résultat
    result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed\n");
    }

    // Affichage des statistiques
    printf("%-15s\n", "Réservations");
    printf("---------------------------\n");

    // Parcours des lignes du résultat
    while ((row = mysql_fetch_row(result)) != NULL) {
        printf("%-15s\n", row[0]);
        printf("\n");
    }

    // Libération de la mémoire du résultat
    mysql_free_result(result);
}

void consultation_stat(MYSQL *conn, int user_type) {
    switch (user_type) 
    {
        case 1: // admin general
            printf("Veuillez choisir de consulter les statistiques pour les 3 sites ou par site : 1 / 2 : ");
            int choix;
            scanf("%d", &choix);
            system("clear");
            switch (choix) 
            {
                case 1: // consultation stat pour les 3 sites
                    consultation_stat_3site(conn);
                    break;
                case 2: // consultation stat par site
                    consultation_stat_site(conn, user_type);
                    break;
                default: // choix invalide
                    consultation_stat(conn, user_type);
                    printf("Entrée erronée");
                    break;
            }
            break;
        case 2: // adminsite
            consultation_stat_site(conn, user_type);
            break;
        default: // autre
            consultation_stat(conn, user_type);
            printf("Entrée erronée");
            break;
    }
}