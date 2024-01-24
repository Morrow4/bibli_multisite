#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_inscrit_bibliotheque(MYSQL *conn, char *username)
{
    system("clear");
    int choix_user;
    printf("               __________________   __________________\n");
    printf("           .-/|                  \\ /                 |\\-.\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |       ~~*~~       ||||\n");
    printf("           ||||    --==*==--      |                   ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |     --==*==--     ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           ||||                   |                   ||||\n");
    printf("           |||'__________________ | __________________'|||\n");
    printf("           ||/===================\\|/===================\\||\n");
    printf("           `--------------------~___~-------------------''\n");

    while (choix_user != 6)
    {
        printf("+------------------------- Bienvenue ! -------------------------+\n");
        printf("|                 Que souhaitez-vous faire ?                    |\n");
        printf("|---------------------------------------------------------------|\n");
        printf("| 1) Lister les livres et leur disponibilité dans les sites     |\n");
        printf("| 2) Emprunter un livre                                         |\n");
        printf("| 3) Réserver un livre                                          |\n");
        printf("| 4) Voir mes réservations                                      |\n");
        printf("| 5) Voir mes emprunts                                          |\n");
        printf("| 6) Déconnexion                                                |\n");
        printf("+---------------------------------------------------------------+\n");
        printf("Veuillez entrer le numéro du choix correspondant : \n");

        scanf("%d", &choix_user);

        switch (choix_user)
        {
        case 1:
            afficher_tous_les_livres(conn);
            break;

        case 2:
            emprunter_livre(conn, username);
            break;

        case 3:
            reserver_livre(conn, username);
            break;

        case 4:
            system("clear");
            afficher_reservations_utilisateur(conn, username);
            break;

        case 5:
            system("clear");
            afficher_emprunts_non_restitues_utilisateur(conn, username);
            break;

        case 6:
            deconnexion(conn);
            break;

        default:
            //choix_inscrit_bibliotheque(conn, username);
            break;
        }
    }
}
