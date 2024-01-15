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
        fprintf(stderr, "Erreur lors de l'ajout du livre : %s\n", mysql_error(conn));
        return;
    }

    printf("Livre ajouté avec succès !\n");
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
        fprintf(stderr, "Erreur lors de la mise à jour du livre : %s\n", mysql_error(conn));
        return;
    }

    printf("Livre mis à jour avec succès !\n");
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

    printf("Livre supprimé avec succès !\n");
}