#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

void ajout_livre(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[5];
    char ISBN[14];
    char titre[255];
    char auteur[101];
    char edition[101];
    char genre[101];

    printf("Veuillez saisir les informations du livre :\n\n");

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

    printf("Titre : ");
    scanf(" %254[^\n]", titre);
    getchar();

    printf("Auteur : ");
    scanf(" %100[^\n]", auteur);
    getchar();

    printf("Edition : ");
    scanf(" %100[^\n]", edition);
    getchar();

    printf("Genre : ");
    scanf(" %100[^\n]", genre);
    getchar();

    // Préparer la requête SQL pour l'ajout du livre
    const char *query = "INSERT INTO Livre (ISBN, Titre, Auteur, Edition, Genre) VALUES (?, ?, ?, ?, ?)";
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

    memset(bind, 0, sizeof(bind));

    // Lier les variables d'entrée à la requête préparée
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = ISBN;
    bind[0].buffer_length = strlen(ISBN);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = titre;
    bind[1].buffer_length = strlen(titre);

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = auteur;
    bind[2].buffer_length = strlen(auteur);

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = edition;
    bind[3].buffer_length = strlen(edition);

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = genre;
    bind[4].buffer_length = strlen(genre);

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

    printf("\nSuccès de l'ajout du livre !\n");

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}

void mise_a_jour_livre(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[5];
    char ISBN[14];
    char titre[255];
    char auteur[101];
    char edition[101];
    char genre[101];
    int choix_recherche = 0;

    while (choix_recherche != 3)
    {
        printf("\n+--------------------------------------------+\n");
        printf("|----Modifier les informations d'un livre----|\n");
        printf("|1) Rechercher l'ISBN du livre par son titre |\n");
        printf("|2) Rechercher l'ISBN du livre par son auteur|\n");
        printf("|3) Modifier les informations du livre       |\n");
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

    printf("\nVeuillez saisir les nouvelles informations du livre :\n\n");

    printf("Nouveau Titre : ");
    scanf(" %254[^\n]", titre);
    getchar();

    printf("Nouvel Auteur : ");
    scanf(" %100[^\n]", auteur);
    getchar();

    printf("Nouvelle Edition : ");
    scanf(" %100[^\n]", edition);
    getchar();

    printf("Nouveau Genre : ");
    scanf(" %100[^\n]", genre);
    getchar();

    // Préparer la requête SQL pour la mise à jour du livre
    const char *query = "UPDATE Livre SET Titre=?, Auteur=?, Edition=?, Genre=? WHERE ISBN=?";
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

    memset(bind, 0, sizeof(bind));

    // Lier les variables d'entrée à la requête préparée
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = titre;
    bind[0].buffer_length = strlen(titre);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = auteur;
    bind[1].buffer_length = strlen(auteur);

    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = edition;
    bind[2].buffer_length = strlen(edition);

    bind[3].buffer_type = MYSQL_TYPE_STRING;
    bind[3].buffer = genre;
    bind[3].buffer_length = strlen(genre);

    bind[4].buffer_type = MYSQL_TYPE_STRING;
    bind[4].buffer = ISBN;
    bind[4].buffer_length = strlen(ISBN);

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

    printf("\nSuccès de la mise à jour du livre !\n");

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}

void suppression_livre(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[1];
    char ISBN[14];
    int choix_recherche = 0;

    while (choix_recherche != 3)
    {
        printf("\n+--------------------------------------------+\n");
        printf("|-------------Supprimer un livre-------------|\n");
        printf("|1) Rechercher l'ISBN du livre par son titre |\n");
        printf("|2) Rechercher l'ISBN du livre par son auteur|\n");
        printf("|3) Supprimer le livre                       |\n");
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
        printf("Veuillez saisir l'ISBN du livre que vous souhaitez supprimer (13 caractères) : ");
        scanf("%13s", ISBN);
        getchar();
        if (strlen(ISBN) != 13)
        {
            printf("L'ISBN doit avoir précisément 13 caractères. Veuillez réessayer.\n");
        }
    } while (strlen(ISBN) != 13);

    // Préparer la requête SQL pour la suppression du livre
    const char *query = "DELETE FROM Livre WHERE ISBN=?";
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

    memset(bind, 0, sizeof(bind));

    // Lier la variable d'entrée à la requête préparée
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = ISBN;
    bind[0].buffer_length = strlen(ISBN);

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

    printf("\nSuccès de la suppression du livre !\n");

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}

// Fonction pour afficher tous les livres avec leur disponibilité
void afficher_tous_les_livres(MYSQL *conn)
{
    // Requête SQL pour récupérer les livres avec leur disponibilité
    const char *query = "SELECT Livre.ISBN, Livre.Titre, Livre.Auteur, Livre.Edition, Livre.Genre, "
                        "CASE WHEN EXISTS (SELECT 1 FROM Exemplaire WHERE Exemplaire.ISBN = Livre.ISBN AND Exemplaire.Disponibilite = true) "
                        "THEN true ELSE false END AS Disponibilite "
                        "FROM Livre";

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la récupération des livres : %s\n", mysql_error(conn));
        return;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);

    // Vérifier si le résultat est valide
    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return;
    }

    // Afficher les en-têtes
    printf("\n+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|%-13s|%-100s|%-50s|%-13s|\n", "ISBN", "Titre", "Auteur", "Disponibilite");
    printf("+-------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------+-------------+\n");

    // Afficher chaque livre avec sa disponibilité
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        // Afficher les détails du livre
        printf("|%-13s|%-100s|%-50s|%-13s|\n", row[0], row[1], row[2], (atoi(row[5]) ? "Disponible" : "Indisponible"));
    }
    printf("+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    // Libérer la mémoire du résultat
    mysql_free_result(result);
}

// Fonction pour échapper les caractères spéciaux dans une chaîne
void escapeString(MYSQL *conn, const char *source, char *destination, size_t dest_size)
{
    mysql_real_escape_string(conn, destination, source, strlen(source));
}

void rechercherLivreParTitre(MYSQL *conn)
{
    char titre[255];

    // Saisie du titre
    printf("Entrez le titre du livre : ");
    scanf(" %254[^\n]", titre);
    getchar();

    // Échapper les caractères spéciaux dans le titre
    char escaped_titre[2 * strlen(titre) + 1];
    escapeString(conn, titre, escaped_titre, sizeof(escaped_titre));

    // Construction de la requête SQL avec la chaîne échappée
    char query[512];
    snprintf(query, sizeof(query), "SELECT Livre.ISBN, Livre.Titre, Livre.Auteur, Exemplaire.ID_Exemplaire, Exemplaire.Disponibilite FROM Livre JOIN Exemplaire ON Livre.ISBN = Exemplaire.ISBN WHERE Livre.Titre LIKE '%%%s%%'", escaped_titre);

    // Exécution de la requête
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_error(conn));
        exit(1);
    }

    // Récupération du résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);

    // Vérification s'il y a des résultats
    if (result == NULL)
    {
        fprintf(stderr, "Aucun livre trouvé avec le titre : %s\n", titre);
        return;
    }

    // Affichage des en-têtes du tableau
    printf("\n+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|%-15s|%-13s|%-100s|%-50s|%-13s|\n", "ID_Exemplaire", "ISBN", "Titre", "Auteur", "Disponibilite");
    printf("+---------------+-------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------+-------------+\n");

    // Parcours des résultats
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        // Affichage des informations de chaque ligne
        printf("|%-15s|%-13s|%-100s|%-50s|%-13s|\n", row[3], row[0], row[1], row[2], row[4]);
    }
    printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    mysql_free_result(result); // Libération de la mémoire du résultat des livres
}

void rechercherLivreParAuteur(MYSQL *conn)
{
    char auteur[101];

    // Saisie du titre
    printf("Entrez le nom de l'auteur : ");
    scanf(" %100[^\n]", auteur);
    getchar();

    // Échapper les caractères spéciaux dans l'auteur
    char escaped_auteur[2 * strlen(auteur) + 1];
    escapeString(conn, auteur, escaped_auteur, sizeof(escaped_auteur));

    // Construction de la requête SQL avec la chaîne échappée
    char query[512];
    snprintf(query, sizeof(query), "SELECT Livre.ISBN, Livre.Titre, Livre.Auteur, Exemplaire.ID_Exemplaire, Exemplaire.Disponibilite FROM Livre JOIN Exemplaire ON Livre.ISBN = Exemplaire.ISBN WHERE Livre.Auteur LIKE '%%%s%%'", escaped_auteur);

    // Exécution de la requête
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête : %s\n", mysql_error(conn));
        exit(1);
    }

    // Récupération du résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);

    // Vérification s'il y a des résultats
    if (result == NULL)
    {
        fprintf(stderr, "Aucun livre trouvé avec l'auteur : %s\n", auteur);
        return;
    }

    // Affichage des en-têtes du tableau
    printf("\n+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|%-15s|%-13s|%-100s|%-50s|%-13s|\n", "ID_Exemplaire", "ISBN", "Titre", "Auteur", "Disponibilite");
    printf("+---------------+-------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------+-------------+\n");

    // Parcours des résultats
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        // Affichage des informations de chaque ligne
        printf("|%-15s|%-13s|%-100s|%-50s|%-13s|\n", row[3], row[0], row[1], row[2], row[4]);
    }
    printf("+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    mysql_free_result(result); // Libération de la mémoire du résultat des livres
}
