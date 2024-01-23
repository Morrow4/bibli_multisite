#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_admin_site_bibliotheque(MYSQL *conn, char *username)
{
    system("clear");
    int choix_user;
    char ISBN[13];
    int id_emprunt = -1;
    char login_utilisateur[100];
    char site_restitution[50];
    printf("\n+--------------------------------------------------------------+\n");
    printf("|                     MENU Administrateur Site                   |\n");
    printf("|--------------------------------------------------------------  |\n");
    printf("| 1)  Lister les livres et leur disponibilité dans les sites     |\n");
    printf("| 2)  Réserver un livre pour soi                                 |\n");
    printf("| 3)  Réserver un livre pour un utilisateur inscrit              |\n");
    printf("| 4)  Ajouter un compte utilisateur                              |\n");
    printf("| 5)  Supprimer un compte utilisateur                            |\n");
    printf("| 6)  Consulter les statistiques du site                         |\n");
    printf("| 7)  Ajouter un livre                                           |\n");
    printf("| 8)  Supprimer un livre                                         |\n");
    printf("| 9)  Bloquer un utilisateur temporairement                      |\n");
    printf("| 10) Valider la restitution d'un livre                          |\n");
    printf("| 11) Emprunter un livre pour soi                                |\n");
    printf("| 12) Emprunter un livre pour un utilisateur inscrit             |\n");
    printf("| 13) Déconnexion                                                |\n");
    printf("+--------------------------------------------------------------+\n\n");
    printf("\nVeuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", &choix_user);

    switch (choix_user)
    {
    case 1:
        // Liste_livres_et_dispo();
        break;

    case 2:
        printf("Veuillez saisir l'ISBN du livre que vous voulez réserver : ");
        scanf("%s", ISBN);
        reserver_livre(conn, username, ISBN);
        break;

    case 3:
        printf("Veuillez saisir le login de l'utilisateur qui veut réserver un livre : ");
        scanf("%s", login_utilisateur);
        printf("Veuillez saisir l'ISBN du livre que vous voulez réserver : ");
        scanf("%s", ISBN);
        reserver_livre(conn, login_utilisateur, ISBN);
        break;

    case 4:
        ajout_compte(conn, username);
        break;

    case 5:
        suppression_compte(conn, username);
        break;

    case 6:
        // Consultation_Stat();
        break;

    case 7:
        ajout_livre(conn);
        break;

    case 8:
        printf("Veuillez saisir l'ISBN du livre que vous voulez supprimer : ");
        scanf("%s", ISBN);
        suppression_livre(conn);
        break;

    case 9:
        blocage_compte(conn, username);
        break;

    case 10:
        printf("Veuillez saisir le numéro d'identification de l'emprunt à restituer : ");
        scanf("%d", &id_emprunt);
        printf("Veuillez saisir le site sur lequel le livre est restitué : ");
        scanf("%s", site_restitution);
        enregistrer_restitution(conn, id_emprunt, site_restitution);
        break;

    case 11:
        Emprunt_soimeme(conn, username);
        break;

    case 12:
        printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre : ");
        scanf("%s", login_utilisateur);
        Emprunt_soimeme(conn, login_utilisateur);
        break;

    case 13:
        deconnexion(conn);
        break;

    default:
        choix_admin_site_bibliotheque(conn, username);
        break;
    }
}
