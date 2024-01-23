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

        printf("Réservation effectuée avec succès !\n");
    }
    else
    {
        printf("Aucun exemplaire disponible pour la réservation.\n");
    }
}

// Fonction pour vérifier si au moins un exemplaire est disponible
bool au_moins_un_exemplaire_disponible(MYSQL *conn, const char *isbn_livre)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[2];
    int disponibilite;

    // Préparer la requête SQL pour vérifier la disponibilité d'au moins un exemplaire
    const char *query = "SELECT COUNT(*) FROM Exemplaire WHERE ISBN=? AND Disponibilite=true";
    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        fprintf(stderr, "\nÉchec de l'initialisation de la requête préparée : %s\n", mysql_error(conn));
        return false;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "\nÉchec de la préparation de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    memset(bind, 0, sizeof(bind));

    // Lier les variables d'entrée et de sortie à la requête préparée
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (void *)isbn_livre;
    bind[0].buffer_length = strlen(isbn_livre);

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &disponibilite;
    bind[1].buffer_length = sizeof(disponibilite);

    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "\nÉchec de la liaison des paramètres : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Exécuter la requête préparée
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "\nErreur lors de l'exécution de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Lier la variable de sortie au résultat de la requête
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &disponibilite;
    bind[0].buffer_length = sizeof(disponibilite);

    if (mysql_stmt_bind_result(stmt, bind))
    {
        fprintf(stderr, "\nÉchec de la liaison des résultats : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return false;
    }

    // Récupérer le résultat de la requête
    if (mysql_stmt_fetch(stmt) == MYSQL_NO_DATA)
    {
        fprintf(stderr, "\nAucune donnée trouvée\n");
        mysql_stmt_close(stmt);
        return false;
    }

    // Fermer la requête préparée
    mysql_stmt_close(stmt);

    return (disponibilite > 0);
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

    char envoi_mail[500];
    sprintf(envoi_mail, "(echo 'Bonjour\n nous vous confirmons la réservation du livre à l'ISBN' '%s' | mail -s 'Confirmation de réservation' '%s')", id_exemplaire, email_utilisateur);
}

// fonction permettant de voir ses réservations
// fonction permettant d'annuler une de ses réservations