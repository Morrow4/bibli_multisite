#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_invite_bibliotheque(MYSQL *conn)
{
    int choix_user;

    printf("+---------------------------Bienvenue !--------------------------+\n\n");
    printf("+--------------------Que souhaitez-vous faire ?--------------------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("2) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", &choix_user);

    switch (choix_user)
    {
    case 1:
        printf("Voici la liste des livres et leur disponibilité dans les différents sites\n");
        // Liste_livres_et_dispo(conn);
        break;

    default:
        choix_invite_bibliotheque(conn);
        break;
    }
}
