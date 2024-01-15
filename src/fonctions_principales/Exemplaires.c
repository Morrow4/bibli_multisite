#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "livre.h"

void ajout_exemplaire(MYSQL *conn)
{
    //Saisie des nouvelles informations de l exemplaire
    char ISBN[13];
    char Disponibilite[1];
    char SitePrincipal[50];
    char EstLivrePourChercheur[1];

    printf("Veuillez saisir les informations de l exemplaire :\n");
    printf("ISBN : ");
    scanf("%12s", ISBN);
    
    printf("Disponibilite : ");
    scanf("%0s", Disponibilite);

    printf("SitePrincipal : ");
    scanf("%49s", SitePrincipal);

    printf("EstLivrePourChercheur : ");
    scanf("%0s", EstLivrePourChercheur);

    // Préparer la requête SQL pour l'ajout de l exemplaire
    char query[1024];
    sprintf(query, "INSERT INTO Exemplaire (ISBN, Disponibilite, SitePrincipal, EstLivrePourChercheur) VALUES ('%s', '%s', '%s', '%s')",
            ISBN, Disponibilite, SitePrincipal, EstLivrePourChercheur);

    // Exécuter la requête SQL
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur lors de l ajout de l exemplaire : %s\n", mysql_error(conn));
        return;
    }

    printf("Succes de l ajout de l exemplaire !\n");
}