#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdbool.h>
#include "fonctions_bdd.h"

// Fonction d'emprunt
void emprunter_livre(MYSQL *conn, char *email_utilisateur, int id_reservation)
{
    // Vérifier si la réservation existe et est encore valide
    if (reservation_existe_et_valide(conn, id_reservation))
    {
        // Obtenir l'ID de l'exemplaire réservé
        int id_exemplaire = obtenir_id_exemplaire_reservé(conn, id_reservation);

        // Mettre à jour le champ EstReserve de la réservation
        mettre_a_jour_est_reserve_reservation(conn, id_reservation, false);

        // Enregistrer l'emprunt dans la table Emprunt
        enregistrer_emprunt(conn, email_utilisateur, id_exemplaire);

        printf("Emprunt effectué avec succès !\n");
    }
    else
    {
        printf("La réservation n'existe pas ou n'est plus valide.\n");
    }
}

// Fonction pour vérifier si la réservation existe et est encore valide
bool reservation_existe_et_valide(MYSQL *conn, int id_reservation)
{
    // Requête SQL pour vérifier si la réservation existe et est encore valide
    char query[1024];
    sprintf(query, "SELECT COUNT(*) FROM Reservation WHERE ID_Reservation=%d AND EstReserve=true", id_reservation);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la vérification de l'existence et de la validité de la réservation : %s\n", mysql_error(conn));
        return false;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    int nombre_reservations_valides = atoi(row[0]);

    // Libérer la mémoire du résultat
    mysql_free_result(result);

    return (nombre_reservations_valides > 0);
}

// Fonction pour obtenir l'ID de l'exemplaire réservé
int obtenir_id_exemplaire_reservé(MYSQL *conn, int id_reservation)
{
    // Requête SQL pour obtenir l'ID de l'exemplaire réservé
    char query[1024];
    sprintf(query, "SELECT ID_Exemplaire FROM Reservation WHERE ID_Reservation=%d", id_reservation);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'obtention de l'ID de l'exemplaire réservé : %s\n", mysql_error(conn));
        return -1;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    int id_exemplaire = atoi(row[0]);

    // Libérer la mémoire du résultat
    mysql_free_result(result);

    return id_exemplaire;
}

// Fonction pour mettre à jour le champ EstReserve de la réservation
void mettre_a_jour_est_reserve_reservation(MYSQL *conn, int id_reservation, bool est_reserve)
{
    // Requête SQL pour mettre à jour le champ EstReserve de la réservation
    char query[1024];
    sprintf(query, "UPDATE Reservation SET EstReserve=%d WHERE ID_Reservation=%d", est_reserve, id_reservation);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la mise à jour du champ EstReserve de la réservation : %s\n", mysql_error(conn));
    }
}

// Fonction pour enregistrer l'emprunt dans la table Emprunt
void enregistrer_emprunt(MYSQL *conn, char *email_utilisateur, int id_exemplaire)
{
    // Requête SQL pour enregistrer l'emprunt
    char query[1024];
    sprintf(query, "INSERT INTO Emprunt (ID_Utilisateur, ID_Exemplaire) VALUES ('%s', %d)", email_utilisateur, id_exemplaire);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'enregistrement de l'emprunt : %s\n", mysql_error(conn));
    }
}
