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

// Fonction pour effectuer l'emprunt
void effectuerEmprunt(MYSQL *conn, char *ISBN, char *username)
{
    qui(username);

    char query[255];
    // Réduire le nombre d'exemplaires disponibles
    // Verifier s'il existe des ISBN pour l'exemplaire demandé
    sprintf(query, "SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN = '%s' AND Disponibilite = true LIMIT 1", ISBN);
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur de vérification\n");
        return;
    }
    // Stocker dans une variable
    int Var_IdExemplaire;
    result = mysql_store_result(conn);
    if ((row = mysql_fetch_row(result)) != NULL)
    {
        sscanf(row[0], "%d", &Var_IdExemplaire);
    }

    mysql_free_result(result);
    // Maj table exemplaire
    sprintf(query, "UPDATE Exemplaire SET Disponibilite = false WHERE ID_Exemplaire = '%d' AND Disponibilite = true", Var_IdExemplaire);
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur lors de la mise à jour du nombre d'exemplaires : %s\n", mysql_error(conn));
        return;
    }

    // récupération de l'ID_Utilisateur = Email
    sprintf(query, "INSERT INTO Emprunt (ID_Exemplaire, ID_Utilisateur) VALUES ('%d', '%s')", Var_IdExemplaire, username);
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur lors de l'ajout de l'emprunt : %s\n", mysql_error(conn));
        return;
    }
    printf("Exemplaire emprunté !\n");
    free((char *)username);
}

// Fonction pour l'emprunt de livre
void emprunter_livre(MYSQL *conn, char *username)
{
    sprintf(username, "%d", getuid());
    int user_group = get_user_group(conn);
    char ISBN[14];
    int choix_recherche = 0;

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        printf("Vous êtes administrateurs général, merci d'utiliser votre compte adhérent. \n");
        break;
    case 2: // adminsite
        printf("Vous êtes administrateurs site, merci d'utiliser votre compte adhérent. \n");
        return;
        break;
    case 3: // adherent
        printf("Bienvenue dans le menu d'emprunt des livres! \n");
    default:
        return;
        break;
    }
    system("clear");

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
