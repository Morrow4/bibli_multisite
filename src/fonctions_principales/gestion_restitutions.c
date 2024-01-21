#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

void verifier_et_valider_restitution(MYSQL *conn, int id_emprunt)
{
    // Requete SQL pour recuperer les informations de la restitution
    char query[1024];
    sprintf(query, "SELECT ID_Emprunt, SiteDeRestitution FROM Restitution WHERE ID_Restitution=%d", id_restitution);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la recuperation des informations de restitution : %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun resultat retourne par la requete\n");
        return;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        fprintf(stderr, "Aucune restitution trouvee avec l ID %d\n", id_restitution);
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

int valider_restitution(MYSQL *conn, int id_emprunt, const char *site_restitution) ///////
{
    // Requete SQL pour recuperer le site principal de l exemplaire
    char query[1024];
    sprintf(query, "SELECT e.SitePrincipal FROM Emprunt em JOIN Exemplaire e ON em.ID_Exemplaire = e.ID_Exemplaire WHERE em.ID_Emprunt=%d", id_emprunt);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la recuperation du site principal de l exemplaire : %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun resultat retourne par la requete\n");
        return 0;
    }

    MYSQL_ROW row = mysql_fetch_row(result);

    if (!row)
    {
        fprintf(stderr, "Aucun site principal trouve pour l exemplaire de l emprunt %d\n", id_emprunt);
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

void marquer_restitution_en_transit(MYSQL *conn, int id_emprunt, const char *site_restitution)
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
        fprintf(stderr, "Erreur lors du marquage de l exemplaire comme disponible : %s\n", mysql_error(conn));
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
        fprintf(stderr, "Erreur lors de la mise a jour de la date de restitution : %s\n", mysql_error(conn));
    }
}

// Ajouter une tache dans le CRON de la forme : 0 0 * * 3 /chemin/vers/votre/programme