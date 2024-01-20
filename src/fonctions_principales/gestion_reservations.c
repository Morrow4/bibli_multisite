#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdbool.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

// Fonction de réservation
void reserver_livre(MYSQL *conn, char *email_utilisateur, char *isbn_livre)
{
    // Vérifier si au moins un exemplaire est disponible
    if (au_moins_un_exemplaire_disponible(conn, isbn_livre))
    {
        // Récupérer l'ID de l'exemplaire disponible
        int id_exemplaire = obtenir_id_exemplaire_disponible(conn, isbn_livre);

        // Mettre à jour le champ EstDisponible de l'exemplaire
        mettre_a_jour_disponibilite_exemplaire(conn, id_exemplaire, false);

        // Enregistrer la réservation dans la table Reservation
        enregistrer_reservation(conn, email_utilisateur, id_exemplaire);

        printf("Réservation effectuée avec succès !\n");
    }
    else
    {
        printf("Aucun exemplaire disponible pour la réservation.\n");
    }
}

// Fonction pour vérifier si au moins un exemplaire est disponible
bool au_moins_un_exemplaire_disponible(MYSQL *conn, char *isbn_livre)
{
    // Requête SQL pour vérifier la disponibilité d'au moins un exemplaire
    char query[1024];
    sprintf(query, "SELECT COUNT(*) FROM Exemplaire WHERE ISBN='%s' AND Disponibilite=true", isbn_livre);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la vérification de la disponibilité des exemplaires : %s\n", mysql_error(conn));
        return false;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    int nombre_exemplaires_disponibles = atoi(row[0]);

    // Libérer la mémoire du résultat
    mysql_free_result(result);

    return (nombre_exemplaires_disponibles > 0);
}

// Fonction pour obtenir l'ID de l'exemplaire disponible
int obtenir_id_exemplaire_disponible(MYSQL *conn, char *isbn_livre)
{
    // Requête SQL pour obtenir l'ID de l'exemplaire disponible
    char query[1024];
    sprintf(query, "SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN='%s' AND Disponibilite=true LIMIT 1", isbn_livre);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'obtention de l'ID de l'exemplaire disponible : %s\n", mysql_error(conn));
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

// Fonction pour mettre à jour le champ EstDisponible de l'exemplaire
void mettre_a_jour_disponibilite_exemplaire(MYSQL *conn, int id_exemplaire, bool disponibilite)
{
    // Requête SQL pour mettre à jour le champ EstDisponible de l'exemplaire
    char query[1024];
    sprintf(query, "UPDATE Exemplaire SET Disponibilite=%d WHERE ID_Exemplaire=%d", disponibilite, id_exemplaire);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la mise à jour de la disponibilité de l'exemplaire : %s\n", mysql_error(conn));
    }
}

// Fonction pour enregistrer la réservation dans la table Reservation
void enregistrer_reservation(MYSQL *conn, char *email_utilisateur, int id_exemplaire)
{
    // Requête SQL pour enregistrer la réservation
    char query[1024];
    sprintf(query, "INSERT INTO Reservation (ID_Utilisateur, ID_Exemplaire, EstReserve) VALUES ('%s', %d, true)", email_utilisateur, id_exemplaire);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'enregistrement de la réservation : %s\n", mysql_error(conn));
    }
}
