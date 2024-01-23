#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <stdbool.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

// Fonction de réservation
void reserver_livre(MYSQL *conn, char *email_utilisateur)
{
    char ISBN[14];
    int choix_recherche = 0;

    while (choix_recherche != 3)
    {
        printf("\n+--------------------------------------------+\n");
        printf("|--------------Réserver un livre-------------|\n");
        printf("|1) Rechercher l'ISBN du livre par son titre |\n");
        printf("|2) Rechercher l'ISBN du livre par son auteur|\n");
        printf("|3) Réserver le livre                        |\n");
        printf("+--------------------------------------------+\n");
        printf("\nVeuillez entrer le numéro du choix correspondant à ce que vous voulez faire : ");
        scanf("%d", &choix_recherche);

        switch (choix_recherche)
        {
        case 1:
            rechercherLivreParTitre(conn);
            break;

        case 2:
            rechercherLivreParAuteur(conn);
            break;

        case 3:
            break;

        default:
            printf("\n+-----------------------------------+\n");
            printf("+Choix invalide. Veuillez réessayer.+\n");
            printf("+-----------------------------------+\n\n");
            break;
        }
    }

    do
    {
        printf("Veuillez saisir l'ISBN du livre que vous souhaitez mettre à jour (13 caractères) : ");
        scanf("%13s", ISBN);
        getchar();
        if (strlen(ISBN) != 13)
        {
            printf("L'ISBN doit avoir précisément 13 caractères. Veuillez réessayer.\n");
        }
    } while (strlen(ISBN) != 13);

    // Vérifier si au moins un exemplaire est disponible
    if (au_moins_un_exemplaire_disponible(conn, ISBN))
    {
        // Récupérer l'ID de l'exemplaire disponible
        int id_exemplaire = obtenir_id_exemplaire_disponible(conn, ISBN);

        // Mettre à jour le champ EstDisponible de l'exemplaire
        mettre_a_jour_disponibilite_exemplaire(conn, id_exemplaire, false);

        // Enregistrer la réservation dans la table Reservation
        enregistrer_reservation(conn, email_utilisateur, id_exemplaire);

        char envoi_mail[500];
        sprintf(envoi_mail, "(echo 'Bonjour\n nous vous confirmons la réservation du livre qui a l'ISBN suivant : ' '%s' | mail -s 'Confirmation de réservation' '%s')", ISBN, email_utilisateur);

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

// Fonction pour afficher toutes les réservations d'un utilisateur
void afficher_reservations_utilisateur(MYSQL *conn, char *email_utilisateur)
{
    // Requête SQL pour récupérer les réservations de l'utilisateur
    char query[1024];
    sprintf(query, "SELECT R.ID_Reservation, L.Titre, L.Auteur, L.ISBN, R.EstReserve, R.DateReservation FROM Reservation R JOIN Exemplaire E ON R.ID_Exemplaire = E.ID_Exemplaire JOIN Livre L ON E.ISBN = L.ISBN WHERE R.ID_Utilisateur = '%s'", email_utilisateur);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la récupération des réservations : %s\n", mysql_error(conn));
        return;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);

    // Afficher les réservations
    printf("\n+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|%-14s|%-100s|%-50s|%-13s|%-19s|%-9s|\n", "ID_Réservation", "Titre", "Auteur", "ISBN", "Date de réservation", "Réservé");
    printf("+--------------|---------------------------------------------------------------------------------------------------|--------------------------------------------------|-------------|-------------------|---------+\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        printf("|%-10s|%-100s|%-50s|%-13s|%-19s|%-9s|\n", row[0], row[1], row[2], row[3], row[5], (atoi(row[4]) ? "Oui" : "Non"));
    }

    printf("+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    // Libérer la mémoire du résultat
    mysql_free_result(result);
}

// Fonction pour annuler une réservation par son ID
void annuler_reservation_par_id(MYSQL *conn, char *email_utilisateur)
{
    int id_reservation;
    int choix_recherche = 0;

    while (choix_recherche != 3)
    {
        printf("\n+-----------------------------------+\n");
        printf("|-------Annuler une réservation-------|\n");
        printf("|1) Voir toutes mes réservations      |\n");
        printf("|2) Annuler la réservation avec son ID|\n");
        printf("+-------------------------------------+\n");
        printf("\nVeuillez entrer le numéro du choix correspondant à ce que vous voulez faire : ");
        scanf("%d", &choix_recherche);

        switch (choix_recherche)
        {
        case 1:
            afficher_reservations_utilisateur(conn, email_utilisateur);
            break;

        case 2:
            break;

        default:
            printf("\n+-----------------------------------+\n");
            printf("+Choix invalide. Veuillez réessayer.+\n");
            printf("+-----------------------------------+\n\n");
            break;
        }
    }

    printf("Veuillez saisir l'ID de la réservation que vous souhaitez supprimer : ");
    scanf("%d", &id_reservation);
    getchar();

    // Vérifier si la réservation existe
    if (!reservation_existe(conn, id_reservation))
    {
        printf("La réservation avec l'ID %d n'existe pas.\n", id_reservation);
        return;
    }

    // Annuler la réservation
    char query[1024];
    sprintf(query, "DELETE FROM Reservation WHERE ID_Reservation = %d", id_reservation);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'annulation de la réservation : %s\n", mysql_error(conn));
        return;
    }

    printf("La réservation avec l'ID %d a été annulée avec succès.\n", id_reservation);
}

// Fonction pour vérifier si une réservation existe
bool reservation_existe(MYSQL *conn, int id_reservation)
{
    // Requête SQL pour vérifier l'existence de la réservation
    char query[1024];
    sprintf(query, "SELECT COUNT(*) FROM Reservation WHERE ID_Reservation = %d", id_reservation);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la vérification de l'existence de la réservation : %s\n", mysql_error(conn));
        return false;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    int nombre_reservations = atoi(row[0]);

    // Libérer la mémoire du résultat
    mysql_free_result(result);

    return (nombre_reservations > 0);
}