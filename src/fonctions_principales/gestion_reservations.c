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
    const char *query = "SELECT COUNT(*) FROM Exemplaire WHERE ISBN=? AND Disponibilite=true";

    // Préparer la requête SQL
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt)
    {
        fprintf(stderr, "Échec de l'initialisation de la requête préparée : %s\n", mysql_error(conn));
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "Échec de la préparation de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Lier les variables d'entrée à la requête préparée
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    int count;
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &count;
    bind[0].is_null = 0;
    bind[0].length = 0;

    if (mysql_stmt_bind_result(stmt, bind))
    {
        fprintf(stderr, "Échec de la liaison des résultats : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Lier la valeur d'ISBN à la requête
    bind[0].buffer = isbn_livre;
    bind[0].buffer_length = strlen(isbn_livre);

    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "Échec de la liaison des paramètres : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Exécuter la requête préparée
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Récupérer le résultat de la requête
    if (mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
    {
        fprintf(stderr, "Aucun résultat trouvé.\n");
        mysql_stmt_close(stmt);
        return false;
    }

    // Libérer la mémoire du résultat
    mysql_stmt_free_result(stmt);

    return (count > 0);
}

// Fonction pour obtenir l'ID de l'exemplaire disponible
int obtenir_id_exemplaire_disponible(MYSQL *conn, char *isbn_livre)
{
    // Requête SQL pour obtenir l'ID de l'exemplaire disponible
    const char *query = "SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN=? AND Disponibilite=true LIMIT 1";

    // Préparer la requête SQL
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt)
    {
        fprintf(stderr, "Échec de l'initialisation de la requête préparée : %s\n", mysql_error(conn));
        return -1;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "Échec de la préparation de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    // Lier les variables d'entrée à la requête préparée
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    int id_exemplaire;
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id_exemplaire;
    bind[0].is_null = 0;
    bind[0].length = 0;

    if (mysql_stmt_bind_result(stmt, bind))
    {
        fprintf(stderr, "Échec de la liaison des résultats : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    // Lier la valeur d'ISBN à la requête
    bind[0].buffer = isbn_livre;
    bind[0].buffer_length = strlen(isbn_livre);

    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "Échec de la liaison des paramètres : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    // Exécuter la requête préparée
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    // Récupérer le résultat de la requête
    if (mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
    {
        fprintf(stderr, "Aucun résultat trouvé.\n");
        mysql_stmt_close(stmt);
        return -1;
    }

    // Libérer la mémoire du résultat
    mysql_stmt_free_result(stmt);

    return id_exemplaire;
}

// Fonction pour mettre à jour le champ EstDisponible de l'exemplaire
void mettre_a_jour_disponibilite_exemplaire(MYSQL *conn, int id_exemplaire, bool disponibilite)
{
    // Requête SQL pour mettre à jour le champ EstDisponible de l'exemplaire
    const char *query = "UPDATE Exemplaire SET Disponibilite=? WHERE ID_Exemplaire=?";

    // Préparer la requête SQL
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt)
    {
        fprintf(stderr, "Échec de l'initialisation de la requête préparée : %s\n", mysql_error(conn));
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "Échec de la préparation de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Lier les variables d'entrée à la requête préparée
    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &disponibilite;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &id_exemplaire;
    bind[1].is_null = 0;
    bind[1].length = 0;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "Échec de la liaison des paramètres : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Exécuter la requête préparée
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}

// Fonction pour enregistrer la réservation dans la table Reservation
void enregistrer_reservation(MYSQL *conn, char *email_utilisateur, int id_exemplaire)
{
    // Requête SQL pour enregistrer la réservation
    const char *query = "INSERT INTO Reservation (ID_Utilisateur, ID_Exemplaire, EstReserve) VALUES (?, ?, true)";

    // Préparer la requête SQL
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt)
    {
        fprintf(stderr, "Échec de l'initialisation de la requête préparée : %s\n", mysql_error(conn));
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "Échec de la préparation de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Lier les variables d'entrée à la requête préparée
    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = email_utilisateur;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &id_exemplaire;
    bind[1].is_null = 0;
    bind[1].length = 0;

    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "Échec de la liaison des paramètres : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Exécuter la requête préparée
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}
