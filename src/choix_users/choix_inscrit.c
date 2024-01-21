#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_inscrit_bibliotheque(MYSQL *conn, char *username)
{
    int choix_user;
    char ISBN[13];

    printf("+---------------------------Bienvenue !--------------------------+\n\n");
    printf("+--------------------Que souhaitez-vous faire ?--------------------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("2) Emprunter un livre\n");
    printf("3) Réserver un livre\n");
    printf("4) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", &choix_user);

    switch (choix_user)
    {
    case 1:
        Liste_livres_et_dispo(conn);
        break;

    case 2:
        Emprunt_soimeme(conn, username);
        break;

    case 3:
        printf("Veuillez saisir l'ISBN du livre que vous voulez réserver : ");
        scanf("%s", ISBN);
        reserver_livre(conn, username, ISBN);
        break;

    case 13:
        deconnexion(conn);
        break;

    default:
        choix_inscrit_bibliotheque(conn, username);
        break;
    }
}
