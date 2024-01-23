#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_admin_general_bibliotheque(MYSQL *conn, char *username)
{
    // system("clear");
    int choix_user = 0;
    int id_emprunt = -1;
    char login_utilisateur[100];
    char site_restitution[50];

    while (choix_user != 17) // L'option de déconnexion est le choix 17
    {
        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         MENU PRINCIPAL                         |\n");
        printf("|                                                                |\n");
        printf("|-------------------- Administrateur Général --------------------|\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 1)  Lister les livres et leur disponibilité dans les sites     |\n");
        printf("| 2)  Réserver un livre pour soi                                 |\n"); // a retirer
        printf("| 3)  Réserver un livre pour un utilisateur inscrit              |\n"); // a retirer
        printf("| 4)  Ajouter un compte utilisateur ou administrateur            |\n");
        printf("| 5)  Supprimer un compte utilisateur ou administrateur          |\n");
        printf("| 6)  Consulter les statistiques globales de la bibliothèque     |\n");
        printf("| 7)  Ajouter un livre                                           |\n");
        printf("| 8)  Mettre à jour les informations d'un livre                  |\n");
        printf("| 9)  Supprimer un livre                                         |\n");
        printf("| 10) Bloquer un utilisateur temporairement                      |\n");
        printf("| 11) Valider la restitution d'un livre                          |\n");
        printf("| 12) Emprunter un livre pour soi                                |\n"); // a retirer
        printf("| 13) Emprunter un livre pour un utilisateur inscrit             |\n"); // a retirer
        printf("| 14) Ajouter un exemplaire                                      |\n");
        printf("| 15) Mettre à jour les informations d'un exemplaire             |\n");
        printf("| 16) Supprimer un exemplaire                                    |\n");
        printf("| 17) Déconnexion                                                |\n");
        printf("| 18) Annuler une réservation                                    |\n");
        printf("| 19) Voir mes réservations                                      |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("\nVeuillez entrer le numéro du choix correspondant à ce que vous voulez faire : ");
        scanf("%d", &choix_user);
        printf("\n\n");

        switch (choix_user)
        {
        case 1:
            afficher_tous_les_livres(conn);
            break;

        case 2:
            system("clear");
            reserver_livre(conn, username);
            break;

        case 3:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur qui veut réserver un livre : ");
            scanf("%s", login_utilisateur);
            reserver_livre(conn, login_utilisateur);
            break;

        case 4:
            ajout_compte(conn, username);
            system("clear");
            break;

        case 5:
            suppression_compte(conn, username);
            system("clear");
            break;

        case 6:
            consultation_stat(conn);
            system("clear");
            break;

        case 7:
            ajout_livre(conn);
            system("clear");
            break;

        case 8:
            mise_a_jour_livre(conn);
            system("clear");
            break;

        case 9:
            suppression_livre(conn);
            system("clear");
            break;

        case 10:
            blocage_compte(conn, username);
            system("clear");
            break;

        case 11:
            printf("Veuillez saisir le numéro d'identification de l'emprunt à restituer : ");
            scanf("%d", &id_emprunt);
            printf("Veuillez saisir le site sur lequel le livre est restitué : ");
            scanf("%s", site_restitution);
            enregistrer_restitution(conn, id_emprunt, site_restitution);
            system("clear");
            break;

        case 12:
            Emprunt_soimeme(conn, username);
            system("clear");
            break;

        case 13:
            printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre : ");
            scanf("%s", login_utilisateur);
            Emprunt_soimeme(conn, login_utilisateur);
            system("clear");
            break;

        case 14:
            ajout_exemplaire(conn);
            system("clear");
            break;

        case 15:
            mise_a_jour_exemplaire(conn);
            system("clear");
            break;

        case 16:
            suppression_exemplaire(conn);
            system("clear");
            break;

        case 17:
            deconnexion(conn);
            system("clear");
            break;

        case 18:
            annuler_reservation_par_id(conn, username);
            break;

        case 19:
            afficher_reservations_utilisateur(conn, username);
            break;

        default:
            printf("\n+-----------------------------------+\n");
            printf("+Choix invalide. Veuillez réessayer.+\n");
            printf("+-----------------------------------+\n\n");
            system("clear");
            break;
        }
    }
}