#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <pwd.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

// Déclaration de variables globales
MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *conn;

// Fonction pour l'emprunt de livre
void emprunter_livre(MYSQL *conn, char *username)
{
    system("clear");

    char ISBN[14];
    int choix_recherche = 0;

    while (choix_recherche != 3)
    {
        printf("\n+--------------------------------------------+\n");
        printf("|-------------Emprunter un livre-------------|\n");
        printf("|1) Rechercher l'ISBN du livre par son titre |\n");
        printf("|2) Rechercher l'ISBN du livre par son auteur|\n");
        printf("|3) Emprunter le livre avec son ISBN         |\n");
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
        printf("ISBN (13 caractères) : ");
        scanf("%13s", ISBN);
        getchar();
        if (strlen(ISBN) != 13)
        {
            printf("L'ISBN doit avoir précisément 13 caractères. Veuillez réessayer.\n");
        }
    } while (strlen(ISBN) != 13);

    effectuerEmprunt(conn, ISBN, username);
}

void effectuerEmprunt(MYSQL *conn, char *ISBN, char *username)
{
    // Vérifier si l'ISBN existe dans la base de données
    if (!ISBNExiste(conn, ISBN))
    {
        printf("L'ISBN %s n'existe pas dans la base de données.\n", ISBN);
        return;
    }

    // Récupérer un exemplaire disponible de l'ISBN demandé
    int id_exemplaire = obtenirIdExemplaireDisponible(conn, ISBN);

    if (id_exemplaire == -1)
    {
        printf("Aucun exemplaire disponible pour l'ISBN %s.\n", ISBN);
        return;
    }

    // Mettre à jour le champ Disponibilite de l'exemplaire à false
    mettreAJourDisponibiliteExemplaire(conn, id_exemplaire, false);

    // Insérer une nouvelle ligne dans la table Emprunt
    char query[1024];
    sprintf(query, "INSERT INTO Emprunt (ID_Exemplaire, ID_Utilisateur) VALUES (%d, '%s')", id_exemplaire, username);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'enregistrement de l'emprunt : %s\n", mysql_error(conn));
        // En cas d'erreur, remettre le champ Disponibilite de l'exemplaire à true
        mettreAJourDisponibiliteExemplaire(conn, id_exemplaire, true);
        return;
    }

    printf("Emprunt du livre avec ISBN %s effectué avec succès.\n", ISBN);
}

// Fonction pour vérifier si un ISBN existe dans la table Livre
int ISBNExiste(MYSQL *conn, char *ISBN)
{
    char query[1024];
    sprintf(query, "SELECT COUNT(*) FROM Livre WHERE ISBN = '%s'", ISBN);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la vérification de l'existence de l'ISBN : %s\n", mysql_error(conn));
        return 0; // Considérer que l'ISBN n'existe pas en cas d'erreur
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return 0; // Considérer que l'ISBN n'existe pas en cas d'erreur
    }

    MYSQL_ROW row = mysql_fetch_row(result);
    int count = atoi(row[0]);

    mysql_free_result(result);

    return count > 0;
}

// Fonction pour obtenir l'ID d'un exemplaire disponible associé à un ISBN
int obtenirIdExemplaireDisponible(MYSQL *conn, char *ISBN)
{
    char query[1024];
    sprintf(query, "SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN = '%s' AND Disponibilite = true LIMIT 1", ISBN);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'obtention de l'ID de l'exemplaire disponible : %s\n", mysql_error(conn));
        return -1;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return -1;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        fprintf(stderr, "Aucun exemplaire disponible trouvé pour l'ISBN %s\n", ISBN);
        mysql_free_result(result);
        return -1;
    }

    int id_exemplaire = atoi(row[0]);

    mysql_free_result(result);

    return id_exemplaire;
}

// Fonction pour mettre à jour le champ Disponibilite d'un exemplaire
void mettreAJourDisponibiliteExemplaire(MYSQL *conn, int id_exemplaire, int disponibilite)
{
    char query[1024];
    sprintf(query, "UPDATE Exemplaire SET Disponibilite = %d WHERE ID_Exemplaire = %d", disponibilite, id_exemplaire);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la mise à jour de la disponibilité de l'exemplaire : %s\n", mysql_error(conn));
    }
}

void afficher_emprunts_non_restitues_utilisateur(MYSQL *conn, char *email_utilisateur)
{
    // Requête SQL pour récupérer les emprunts non restitués de l'utilisateur
    const char *query = "SELECT Emprunt.ID_Emprunt, Livre.Titre, Livre.Auteur, Emprunt.DateEmprunt FROM Emprunt JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire JOIN Livre ON Exemplaire.ISBN = Livre.ISBN WHERE Emprunt.ID_Utilisateur = ? AND Emprunt.DateRestitution IS NULL";

    // Préparer la requête SQL
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[1];
    memset(bind, 0, sizeof(bind));

    stmt = mysql_stmt_init(conn);

    if (!stmt)
    {
        fprintf(stderr, "\nÉchec de l'initialisation de la requête préparée : %s\n", mysql_error(conn));
        return;
    }

    if (mysql_stmt_prepare(stmt, query, strlen(query)))
    {
        fprintf(stderr, "\nÉchec de la préparation de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Lier les variables d'entrée à la requête préparée
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = email_utilisateur;
    bind[0].buffer_length = strlen(email_utilisateur);

    if (mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "\nÉchec de la liaison des paramètres : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Exécuter la requête préparée
    if (mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "\nErreur lors de l'exécution de la requête : %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_stmt_result_metadata(stmt);

    // Vérifier si le résultat est valide
    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        mysql_stmt_close(stmt);
        return;
    }

    // Afficher les en-têtes
    printf("\n+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|%-13s|%-100s|%-50s|%-20s|\n", "ID_Emprunt", "Titre", "Auteur", "Date d'emprunt");
    printf("+-------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------+--------------------+\n");

    // Afficher chaque emprunt non restitué de l'utilisateur
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        // Afficher les détails de l'emprunt
        printf("|%-13s|%-100s|%-50s|%-20s|\n", row[0], row[1], row[2], row[3]);
    }
    printf("n+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    // Libérer la mémoire du résultat
    mysql_free_result(result);

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}
