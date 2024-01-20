#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livre.h"

void ajout_livre(MYSQL *conn)
{
    // Saisie des informations du livre
    char ISBN[13];
    char titre[255];
    char auteur[100];
    char edition[100];
    char genre[100];

    printf("Veuillez saisir les informations du livre :\n");
    printf("ISBN : ");
    scanf("%12s", ISBN);

    printf("Titre : ");
    scanf("%254s", titre);

    printf("Auteur : ");
    scanf("%99s", auteur);

    printf("Edition : ");
    scanf("%99s", edition);

    printf("Genre : ");
    scanf("%99s", genre);

    // Préparer la requête SQL pour l'ajout du livre
    char query[1024];
    sprintf(query, "INSERT INTO Livre (ISBN, Titre, Auteur, Edition, Genre) VALUES ('%s', '%s', '%s', '%s', '%s')",
            ISBN, titre, auteur, edition, genre);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l ajout du livre : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de l ajout du livre !\n");
}

void mise_a_jour_livre(MYSQL *conn, char *ISBN)
{
    // Saisie des nouvelles informations du livre
    char titre[255];
    char auteur[100];
    char edition[100];
    char genre[100];

    printf("Veuillez saisir les nouvelles informations du livre :\n");

    printf("Nouveau Titre : ");
    scanf("%254s", titre);

    printf("Nouvel Auteur : ");
    scanf("%99s", auteur);

    printf("Nouvelle Edition : ");
    scanf("%99s", edition);

    printf("Nouveau Genre : ");
    scanf("%99s", genre);

    // Préparer la requête SQL pour la mise à jour du livre
    char query[1024];
    sprintf(query, "UPDATE Livre SET Titre='%s', Auteur='%s', Edition='%s', Genre='%s' WHERE ISBN='%s'",
            titre, auteur, edition, genre, ISBN);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la mise a jour du livre : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de la mise a jour du livre !\n");
}

void suppression_livre(MYSQL *conn, char *ISBN)
{
    // Préparer la requête SQL pour la suppression du livre
    char query[1024];
    sprintf(query, "DELETE FROM Livre WHERE ISBN='%s'", ISBN);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la suppression du livre : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de la suppression du livre !\n");
}

void recherche_ISBN(MYSQL *conn, char *titre, char *auteur)
{
    // Préparer la requête SQL pour la recherche d'ISBN par titre ou auteur
    char query[1024];
    sprintf(query, "SELECT ISBN FROM Livre WHERE Titre='%s' OR Auteur='%s'", titre, auteur);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de la recherche d'ISBN : %s\n", mysql_error(conn));
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);

    if (!result)
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return;
    }

    // Afficher les ISBNs trouvés
    printf("Résultats de la recherche d'ISBN pour le livre avec titre \"%s\" ou auteur \"%s\" :\n", titre, auteur);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(result)) != NULL)
    {
        printf("ISBN: %s\n", row[0]);
        printf("------------------------------\n");
    }

    mysql_free_result(result);
}