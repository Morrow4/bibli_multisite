#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

void ajout_exemplaire(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[3];
    char ISBN[14];
    char site_principal[51];
    char chercheur_str[6];
    bool is_valid_site = false;
    bool is_valid_chercheur = false;
    int choix_recherche;

    while (choix_recherche != 3)
    {
        printf("\n+--------------------------------------------+\n");
        printf("|------Ajouter un exemplaire d'un livre------|\n");
        printf("|1) Rechercher l'ISBN du livre par son titre |\n");
        printf("|2) Rechercher l'ISBN du livre par son auteur|\n");
        printf("|3) Saisir les informations de l'exemplaire  |\n");
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

    printf("Veuillez saisir les informations de l'exemplaire :\n\n");

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

    // Saisir le site principal avec vérification
    do
    {
        printf("Nouveau site principal (Site A, Site B, ou Site C) : ");
        scanf(" %50[^\n]", site_principal);
        getchar();

        if (strcmp(site_principal, "Site A") == 0 || strcmp(site_principal, "Site B") == 0 || strcmp(site_principal, "Site C") == 0)
        {
            is_valid_site = true;
        }
        else
        {
            printf("Veuillez saisir un site principal valide.\n");
        }
    } while (!is_valid_site);

    // Saisir la variable chercheur_str avec vérification
    do
    {
        printf("Est un livre pour chercheur (vrai/faux) : ");
        scanf(" %5[^\n]", chercheur_str);
        getchar();

        if (strcmp(chercheur_str, "vrai") == 0 || strcmp(chercheur_str, "faux") == 0)
        {
            is_valid_chercheur = true;
        }
        else
        {
            printf("Veuillez saisir une valeur valide pour chercheur (vrai/faux).\n");
        }
    } while (!is_valid_chercheur);

    // Convertir les chaînes en valeurs booléennes
    int chercheur = (strcmp(chercheur_str, "vrai") == 0) ? 1 : 0;

    // Préparer la requête SQL pour l'ajout de l'exemplaire
    const char *query = "INSERT INTO Exemplaire (ISBN, SitePrincipal, EstLivrePourChercheur) VALUES (?, ?, ?)";
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
    bind[1].buffer = site_principal;
    bind[1].buffer_length = strlen(site_principal);

    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &chercheur;
    bind[2].buffer_length = sizeof(chercheur);

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

    printf("\nSuccès de l'ajout de l'exemplaire !\n");

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}

void mise_a_jour_exemplaire(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[4];
    int id_exemplaire;
    char site_principal[51];
    char disponibilite_str[6];
    char chercheur_str[6];
    bool is_valid_site = false;
    bool is_valid_disponibilite = false;
    bool is_valid_chercheur = false;
    int choix_recherche;

    while (choix_recherche != 3)
    {
        printf("\n+-----------------------------------------------------------+\n");
        printf("|--------------Modifier l'exemplaire d'un livre-------------|\n");
        printf("|1) Rechercher l'ID de l'exemplaire du livre par son titre  |\n");
        printf("|2) Rechercher l'ID' de l'exemplaire du livre par son auteur|\n");
        printf("|3) Saisir les nouvelles informations de l'exemplaire       |\n");
        printf("+-----------------------------------------------------------+\n");
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

    // Saisir l'ID de l'exemplaire
    printf("Veuillez saisir l'ID de l'exemplaire : ");
    scanf("%d", &id_exemplaire);
    getchar();

    // Saisir les nouvelles informations de l'exemplaire
    printf("\nVeuillez saisir les nouvelles informations de l'exemplaire :\n\n");

    // Saisir le site principal avec vérification
    do
    {
        printf("Nouveau site principal (Site A, Site B, ou Site C) : ");
        scanf(" %50[^\n]", site_principal);
        getchar();

        if (strcmp(site_principal, "Site A") == 0 || strcmp(site_principal, "Site B") == 0 || strcmp(site_principal, "Site C") == 0)
        {
            is_valid_site = true;
        }
        else
        {
            printf("Veuillez saisir un site principal valide.\n");
        }
    } while (!is_valid_site);

    // Saisir la disponibilité avec vérification
    do
    {
        printf("Nouvelle disponibilité (vrai/faux) : ");
        scanf(" %5[^\n]", disponibilite_str);
        getchar();

        if (strcmp(disponibilite_str, "vrai") == 0 || strcmp(disponibilite_str, "faux") == 0)
        {
            is_valid_disponibilite = true;
        }
        else
        {
            printf("Veuillez saisir une disponibilité valide (vrai/faux).\n");
        }
    } while (!is_valid_disponibilite);

    // Saisir la variable chercheur_str avec vérification
    do
    {
        printf("Est un livre pour chercheur (vrai/faux) : ");
        scanf(" %5[^\n]", chercheur_str);
        getchar();

        if (strcmp(chercheur_str, "vrai") == 0 || strcmp(chercheur_str, "faux") == 0)
        {
            is_valid_chercheur = true;
        }
        else
        {
            printf("Veuillez saisir une valeur valide pour chercheur (vrai/faux).\n");
        }
    } while (!is_valid_chercheur);

    // Convertir les chaînes en valeurs booléennes
    int disponibilite = (strcmp(disponibilite_str, "vrai") == 0) ? 1 : 0;
    int est_chercheur = (strcmp(chercheur_str, "vrai") == 0) ? 1 : 0;

    // Préparer la requête SQL pour la mise à jour de l'exemplaire
    const char *query = "UPDATE Exemplaire SET SitePrincipal=?, Disponibilite=?, EstLivrePourChercheur=? WHERE ID_Exemplaire=?";
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
    bind[0].buffer = site_principal;
    bind[0].buffer_length = strlen(site_principal);

    bind[1].buffer_type = MYSQL_TYPE_LONG;
    bind[1].buffer = &disponibilite;
    bind[1].buffer_length = sizeof(disponibilite);

    bind[2].buffer_type = MYSQL_TYPE_LONG;
    bind[2].buffer = &est_chercheur;
    bind[2].buffer_length = sizeof(est_chercheur);

    bind[3].buffer_type = MYSQL_TYPE_LONG;
    bind[3].buffer = &id_exemplaire;
    bind[3].buffer_length = sizeof(id_exemplaire);

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

    printf("\nSuccès de la mise à jour de l'exemplaire !\n");

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}

void suppression_exemplaire(MYSQL *conn)
{
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[1];
    int id_exemplaire;
    int choix_recherche;

    while (choix_recherche != 3)
    {
        printf("\n+-----------------------------------------------------------+\n");
        printf("|-------------Supprimer l'exemplaire d'un livre-------------|\n");
        printf("|1) Rechercher l'ID de l'exemplaire du livre par son titre  |\n");
        printf("|2) Rechercher l'ID' de l'exemplaire du livre par son auteur|\n");
        printf("|3) Supprimer l'exemplaire                                  |\n");
        printf("+-----------------------------------------------------------+\n");
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

    // Saisir l'ID de l'exemplaire
    printf("Veuillez saisir l'ID de l'exemplaire à supprimer : ");
    scanf("%d", &id_exemplaire);
    getchar();

    // Préparer la requête SQL pour la suppression de l'exemplaire
    const char *query = "DELETE FROM Exemplaire WHERE ID_Exemplaire=?";
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
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = &id_exemplaire;
    bind[0].buffer_length = sizeof(id_exemplaire);

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

    printf("\nSuccès de la suppression de l'exemplaire !\n");

    // Fermer la requête préparée
    mysql_stmt_close(stmt);
}
