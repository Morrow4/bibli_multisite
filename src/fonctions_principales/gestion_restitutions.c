#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

// Fonction pour enregistrer une nouvelle restitution dans la base de données
void enregistrer_restitution(MYSQL *conn, char *email_utilisateur)
{
    int id_emprunt;
    char *site_restitution;
    int choix_recherche = 0;

    while (choix_recherche != 2)
    {
        printf("\n+--------------------------------------------+\n");
        printf("|---------Restituer un livre emprunté--------|\n");
        printf("|1) Voir tous mes emprunts non restitués     |\n");
        printf("|2) Restituer un livre avec l'ID de l'emprunt|\n");
        printf("+--------------------------------------------+\n");
        printf("\nVeuillez entrer le numéro du choix correspondant à ce que vous voulez faire : ");
        scanf("%d", &choix_recherche);

        switch (choix_recherche)
        {
        case 1:
            afficher_emprunts_non_restitues_utilisateur(conn, email_utilisateur);
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

    printf("\nVeuillez saisir l'ID de l'emprunt lié au livre restitué : ");
    scanf("%d", &id_emprunt);
    getchar();

    printf("\nVeuillez saisir le site dans lequel le est livre restitué : ");
    scanf("%s", site_restitution);
    getchar();

    // Requête SQL pour insérer une nouvelle restitution
    char query[1024];
    sprintf(query, "INSERT INTO Restitution (ID_Emprunt, SiteDeRestitution) VALUES (%d, '%s')", id_emprunt, site_restitution);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'enregistrement de la restitution : %s\n", mysql_error(conn));
        return;
    }

    // Récupérer l'ID de la restitution nouvellement créée
    int id_restitution = mysql_insert_id(conn);

    verifier_et_valider_restitution(conn, id_restitution);
}

void verifier_et_valider_restitution(MYSQL *conn, int id_restitution)
{
    // Requete SQL pour recuperer les informations de la restitution
    char query[1024];
    sprintf(query, "SELECT ID_Emprunt, SiteDeRestitution FROM Restitution WHERE ID_Restitution=%d", id_restitution);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la récuperation des informations de restitution : %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        fprintf(stderr, "Aucune restitution trouvée avec l'ID %d\n", id_restitution);
        mysql_free_result(result);
        return;
    }

    // Recuperer les informations de la restitution
    int id_emprunt = atoi(row[0]);
    char site_restitution[50];
    strcpy(site_restitution, row[1]);

    // Liberer le resultat
    mysql_free_result(result);

    // Verifier si le livre a ete rendu dans son site principal
    if (valider_restitution(conn, id_emprunt, site_restitution))
    {
        // Si la restitution est validee, marquer l exemplaire comme disponible
        marquer_exemplaire_disponible(conn, id_emprunt);

        // Mettre a jour la date de restitution
        mettre_a_jour_date_restitution(conn, id_restitution);
    }
}

int valider_restitution(MYSQL *conn, int id_emprunt, char *site_restitution)
{
    // Requete SQL pour recuperer le site principal de l exemplaire
    char query[1024];
    sprintf(query, "SELECT e.SitePrincipal FROM Emprunt em JOIN Exemplaire e ON em.ID_Exemplaire = e.ID_Exemplaire WHERE em.ID_Emprunt=%d", id_emprunt);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la recuperation du site principal de l'exemplaire : %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return 0;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        fprintf(stderr, "Aucun site principal trouvé pour l'exemplaire de l'emprunt %d\n", id_emprunt);
        mysql_free_result(result);
        return 0;
    }

    // Recuperer le site principal de l exemplaire
    char site_principal[50];
    strcpy(site_principal, row[0]);

    // Liberer le resultat
    mysql_free_result(result);

    // Verifier si le livre a ete rendu dans son site principal
    if (strcmp(site_principal, site_restitution) == 0)
    {
        // Si le livre a ete rendu dans son site principal, la restitution est validee
        return 1;
    }
    else
    {
        // Si le livre n a pas ete rendu dans son site principal, marquer comme "en transit"
        marquer_restitution_en_transit(conn, id_emprunt, site_restitution);
        return 0;
    }
}

void marquer_restitution_en_transit(MYSQL *conn, int id_emprunt, char *site_restitution)
{
    // Requete SQL pour marquer la restitution comme "en transit"
    char query[1024];
    sprintf(query, "UPDATE Restitution SET EnTransit=true WHERE ID_Emprunt=%d AND SiteDeRestitution='%s'", id_emprunt, site_restitution);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors du marquage de la restitution comme \"en transit\" : %s\n", mysql_error(conn));
    }
}

void marquer_exemplaire_disponible(MYSQL *conn, int id_emprunt)
{
    // Requete SQL pour marquer l exemplaire comme disponible
    char query[1024];
    sprintf(query, "UPDATE Exemplaire SET Disponibilite=true WHERE ID_Exemplaire=(SELECT ID_Exemplaire FROM Emprunt WHERE ID_Emprunt=%d)", id_emprunt);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors du marquage de l'exemplaire comme disponible : %s\n", mysql_error(conn));
    }
}

// Fonction pour mettre a jour la date de restitution
void mettre_a_jour_date_restitution(MYSQL *conn, int id_restitution)
{
    // Requete SQL pour mettre a jour la date de restitution
    char query[1024];
    sprintf(query, "UPDATE Restitution SET DateRestitution=NOW() WHERE ID_Restitution=%d", id_restitution);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la mise à jour de la date de restitution : %s\n", mysql_error(conn));
    }
}

void afficher_livres_restitues_apres_transit(MYSQL *conn)
{
    // Requête SQL pour récupérer les livres avec ValiderApresTransit à true
    const char *query = "SELECT Restitution.ID_Restitution, Livre.Titre, Livre.Auteur, Restitution.DateRestitution FROM Restitution JOIN Emprunt ON Restitution.ID_Emprunt = Emprunt.ID_Emprunt JOIN Exemplaire ON Emprunt.ID_Exemplaire = Exemplaire.ID_Exemplaire JOIN Livre ON Exemplaire.ISBN = Livre.ISBN WHERE Restitution.ValiderApresTransit = true";

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la récupération des livres à valider après transit : %s\n", mysql_error(conn));
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
    printf("\n+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");
    printf("|%-13s|%-100s|%-50s|%-20s|\n", "ID_Restitution", "Titre", "Auteur", "Date de restitution");
    printf("+-------------+----------------------------------------------------------------------------------------------------+--------------------------------------------------+--------------------+\n");

    // Afficher chaque livre à valider après transit
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)))
    {
        // Afficher les détails du livre à valider
        printf("|%-13s|%-100s|%-50s|%-20s|\n", row[0], row[1], row[2], row[3]);
    }
    printf("+------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+\n");

    // Libérer la mémoire du résultat
    mysql_free_result(result);
}

void valider_restitution_apres_transit(MYSQL *conn)
{
    int id_restitution;
    int choix_recherche = 0;

    while (choix_recherche != 2)
    {
        printf("\n+--------------------------------------------------------------------------+\n");
        printf("|----------------Valider la restitution d'un livre rappatrié---------------|\n");
        printf("|1) Voir tous les livres rappatriés nécessitant une validation             |\n");
        printf("|2) Valider la restitution d'un livre rappatrié avec l'ID de la restitution|\n");
        printf("+--------------------------------------------------------------------------+\n");
        printf("\nVeuillez entrer le numéro du choix correspondant à ce que vous voulez faire : ");
        scanf("%d", &choix_recherche);

        switch (choix_recherche)
        {
        case 1:
            afficher_livres_restitues_apres_transit(conn);
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

    printf("\nVeuillez saisir l'ID de la restitution à valider : ");
    scanf("%d", &id_restitution);
    getchar();

    // Requête SQL pour valider la restitution après transit
    char query[1024];
    sprintf(query, "UPDATE Restitution SET ValiderApresTransit=false, EstRestitue=true WHERE ID_Restitution=%d", id_restitution);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la validation de la restitution après transit : %s\n", mysql_error(conn));
        return;
    }

    // Récupérer l'ID de l'emprunt associé à la restitution
    int id_emprunt = obtenir_id_emprunt_de_restitution(conn, id_restitution);

    // Marquer l'exemplaire comme disponible
    marquer_exemplaire_disponible(conn, id_emprunt);

    // Mettre à jour la date de restitution
    mettre_a_jour_date_restitution(conn, id_restitution);

    printf("La restitution après transit avec l'ID %d a été validée avec succès.\n", id_restitution);
}

int obtenir_id_emprunt_de_restitution(MYSQL *conn, int id_restitution)
{
    // Requête SQL pour obtenir l'ID de l'emprunt associé à la restitution
    char query[1024];
    sprintf(query, "SELECT ID_Emprunt FROM Restitution WHERE ID_Restitution=%d", id_restitution);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l'obtention de l'ID de l'emprunt associé à la restitution : %s\n", mysql_error(conn));
        return -1;
    }

    // Récupérer le résultat de la requête
    MYSQL_RES *result = mysql_store_result(conn);

    // Vérifier si le résultat est valide
    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return -1;
    }

    // Récupérer l'ID de l'emprunt associé à la restitution
    MYSQL_ROW row = mysql_fetch_row(result);

    // Libérer la mémoire du résultat
    mysql_free_result(result);

    // Vérifier si l'ID a été récupéré
    if (!row)
    {
        fprintf(stderr, "Aucun emprunt associé à la restitution avec l'ID %d\n", id_restitution);
        return -1;
    }

    return atoi(row[0]);
}
