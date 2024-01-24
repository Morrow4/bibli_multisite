#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_invite_bibliotheque(MYSQL *conn)
{
    system("clear");
    int choix_user;
    printf("                             (((\n");
    printf("                            c'',)\n");
    printf("                             <|>\n");
    printf("                            _/\\_\n");
    printf("                           |||  ||\n");
    printf("+---------------------------Bienvenue !--------------------------+\n\n");
    printf("+--------------------Que souhaitez-vous faire ?--------------------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("Appuyez sur n'importe quelle autre touche pour vous déconnecter\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", &choix_user);

    switch (choix_user)
    {
    case 1:
        printf("Voici la liste des livres et leur disponibilité dans les différents sites\n");
        afficher_tous_les_livres(conn);
        break;

    default:
        choix_invite_bibliotheque(conn);
        break;
    }
}
