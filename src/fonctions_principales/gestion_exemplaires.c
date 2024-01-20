#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

void ajout_exemplaire(MYSQL *conn)
{
    // Saisie des nouvelles informations de l exemplaire
    char ISBN[13];
    char site_principal[50];
    char chercheur_str[5];

    printf("Veuillez saisir les informations de l exemplaire :\n");
    printf("ISBN : ");
    scanf("%12s", ISBN);

    printf("site_principal : ");
    scanf("%49s", site_principal);

    printf("Ce livre est pour chercheur (vrai/faux) : ");
    scanf("%4s", chercheur_str);

    // Convertir les chaines en valeurs booleennes
    int chercheur = (strcmp(chercheur_str, "vrai") == 0) ? 1 : 0;

    // Preparer la requete SQL pour l'ajout de l exemplaire
    char query[1024];
    sprintf(query, "INSERT INTO Exemplaire (ISBN, SitePrincipal, EstLivrePourChercheur) VALUES ('%s', '%s', '%s', '%s')",
            ISBN, site_principal, chercheur);

    // Executer la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l ajout de l exemplaire : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de l ajout de l exemplaire !\n");
}

void mise_a_jour_exemplaire(MYSQL *conn, int id_exemplaire)
{
    // Saisie des nouvelles informations de l exemplaire
    char site_principal[50];
    char disponibilite_str[5];
    char chercheur_str[5];

    printf("Veuillez saisir les nouvelles informations de l exemplaire :\n");

    printf("Nouveau Site principal : ");
    scanf("%49s", site_principal);

    printf("Nouvelle Disponibilite (vrai/faux) : ");
    scanf("%4s", disponibilite_str);

    printf("Est un livre pour chercheur (vrai/faux) : ");
    scanf("%4s", chercheur_str);

    // Convertir les chaines en valeurs booleennes
    int disponibilite = (strcmp(disponibilite_str, "vrai") == 0) ? 1 : 0;
    int est_chercheur = (strcmp(chercheur_str, "vrai") == 0) ? 1 : 0;

    // Preparer la requete SQL pour la mise a jour de l exemplaire
    char query[1024];
    sprintf(query, "UPDATE Exemplaire SET SitePrincipal='%s', Disponibilite=%d, EstLivrePourChercheur=%d WHERE ID_Exemplaire=%d",
            site_principal, disponibilite, est_chercheur, id_exemplaire);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la mise a jour de l exemplaire : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de la mise a jour de l exemplaire !\n");
}

void suppression_exemplaire(MYSQL *conn, int id_exemplaire)
{
    // Preparer la requete SQL pour la suppression de l exemplaire
    char query[1024];
    sprintf(query, "DELETE FROM Exemplaire WHERE ID_Exemplaire=%d", id_exemplaire);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la suppression de l exemplaire : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de la suppression de l exemplaire !\n");
}

void recherche_exemplaire(MYSQL *conn, char *ISBN)
{
    // Preparer la requete SQL pour la recherche d exemplaires par ISBN
    char query[1024];
    sprintf(query, "SELECT ID_Exemplaire, SitePrincipal, Disponibilite, EstLivrePourChercheur FROM Exemplaire WHERE ISBN='%s'", ISBN);

    // Executer la requete SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la recherche d exemplaires : %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "La requete ne retourne aucun resultat\n");
        return;
    }

    // Afficher les informations des exemplaires trouves
    printf("Resultats de la recherche d exemplaires pour le livre avec ISBN %s :\n", ISBN);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        printf("ID_Exemplaire: %s\n", row[0]);
        printf("SitePrincipal: %s\n", row[1]);
        printf("Disponibilite: %s\n", row[2]);
        printf("EstLivrePourChercheur: %s\n", row[3]);
        printf("------------------------------\n");
    }

    mysql_free_result(result);
}