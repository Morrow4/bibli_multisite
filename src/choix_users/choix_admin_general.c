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
    char login_utilisateur[100];
    int user_type = 1;

    printf("   ____________________________________________________________________________\n");
    printf("  |____________________________________________________________________________|\n");
    printf("  | __     __   ____   ___ ||  ____    ____     _  __        __   __   _  __  |\n");
    printf("  ||  |__ |--|_| || |_|   |||_|**|*|__|+|+||___| ||  |     _|  |_|  ||#||==|  / /|\n");
    printf("  ||==|^^||--| |=||=| |=*=||| |~~|~|  |=|=|| | |~||==|    | |  | |  || ||==| / / |\n");
    printf("  ||  |##||  | | || | |JRO|||-|  | |==|+|+||-|-|~||__|    | |  | |==|| ||__|/_/__|\n");
    printf("  ||__|__||__|_|_||_|_|___|||_|__|_|__|_|_||_|_|_||__|    | |__|_|__||_||__|/__/\n");
    printf("  ||_______________________||__________________________| |___________________|\n");
    printf("  | _____________________  ||      __   __  _  __    _ | | _______   __   __   |\n");
    printf("  ||=|=|=|=|=|=|=|=|=|=|=| __..\\/ |  |_|  ||#||==|  / /| ||=|=|=|=|_|  |_|  | |\n");
    printf("  || | | | | | | | | | | |\\/\\ \\  \\|++|=|  || ||==| / / | || | | | | | |  | |  | |\n");
    printf("  ||_|_|_|_|_|_|_|_|_|_|_/\\/\\._.__\\__|_|__||_||__|/_/__| ||_|_|_|_|_|_|__|_|__|_|\n");
    printf("  ||____________________ /\\~()/()~\\/\\ __________________| |___________________|\n");
    printf("  | __   __    _  _     \\_  (_ .  _/ _    ___     _____| |__    _  /    ________|\n");
    printf("  ||~~|_|..|__| || |_ _   \\ //\\\\ /  |=|__|~|~|___| | | | |||\\ \\  |=|/   //    /|\n");
    printf("  ||--|+|^^|==|1||2| | |__/\\ __ /\\__| |==|x|x|+|+|=|=|=||| \\ \\/ |*/  .//____//.//\n");
    printf("  ||__|_|__|__|_||_|_| /  \\ \\  / /  \\_|__|_|_|_|_|_|_|_|||__\\_\\|/   /(____|/ //\n");
    printf("  |_________________ _/    \\/\\/\\/    \\_ _______________| |___ /   /________//\n");
    printf("  | _____   _   __  |/      \\../      \\|  __   __   ___|     /                  \\\n");
    printf("  ||_____|_| |_|##|_||   |   \\/ __|   ||_|==|_|++|_|-|||    /                    \\\n");
    printf("  ||______||=|#|--| |\\   \\   o    /   /| |  |~|  | | |||   /                      \\\n");
    printf("  ||______||_|_|__|_|_\\   \\  o   /   /_|_|__|_|__|_|_|||  |     ________     ______\\\n");
    printf("  |_________ __________\\___\\____/___/___________ ______| |____|________)     /  /\n");
    printf("  |__    _  /    ________     ______           /| _ _ _|       |_________/   /\n");
    printf("  |\\ \\  |=|/   //    /| //   /  /  / |        /  || | | |                     /\n");
    printf("  | \\ \\/ |*/  .//____//.//   /__/__/ (_)      /  ||=|=|=|                    /\n");
    printf("__|  \\ \\/|/   /(____|/ //                    /  /||~|~|~|__                  /\n");
    printf("  |___\\_/   /________//   ________         /  / ||_|_|_|_|               /\n");
    printf("  |___ /   (|________/   |\\_______\\       /  / | |                       /\n");
    printf("      /                  \\|________)     /  /  | |                     /\n");

    while (choix_user != 17) // L'option de déconnexion est le choix 17
    {
        printf("\n+----------------------------------------------------------------+\n");
        printf("|                         MENU PRINCIPAL                         |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|-------------------- Administrateur Général --------------------|\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 1)  Lister les livres et leur disponibilité dans les sites     |\n");
        printf("| 3)  Réserver un livre pour un utilisateur inscrit              |\n");
        printf("| 4)  Ajouter un compte utilisateur ou administrateur            |\n");
        printf("| 5)  Supprimer un compte utilisateur ou administrateur          |\n");
        printf("| 6)  Consulter les statistiques globales de la bibliothèque     |\n");
        printf("| 7)  Ajouter un livre                                           |\n");
        printf("| 8)  Mettre à jour les informations d'un livre                  |\n");
        printf("| 9)  Supprimer un livre                                         |\n");
        printf("| 10) Bloquer un utilisateur temporairement                      |\n");
        printf("| 11) Enregistrer la restitution d'un livre                      |\n");
        printf("| 13) Emprunter un livre pour un utilisateur inscrit             |\n");
        printf("| 14) Ajouter un exemplaire                                      |\n");
        printf("| 15) Mettre à jour les informations d'un exemplaire             |\n");
        printf("| 16) Supprimer un exemplaire                                    |\n");
        printf("| 17) Déconnexion                                                |\n");
        printf("| 18) Annuler une réservation                                    |\n");
        printf("| 19) Voir mes réservations                                      |\n");
        printf("| 20) Emprunter un livre réservé                                 |\n");
        printf("| 21) Valider la restitution des livres rappatriés               |\n");
        printf("+----------------------------------------------------------------+\n");

        printf("\nVeuillez entrer le numéro du choix correspondant à ce que vous voulez faire : ");
        scanf("%d", &choix_user);
        printf("\n\n");

        switch (choix_user)
        {
        case 1:
            system("clear");
            afficher_tous_les_livres(conn);
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
            system("clear");
            consultation_stat(conn, user_type);
            // system("clear");
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
            system("clear");
            suppression_livre(conn);
            system("clear");
            break;

        case 10:
            system("clear");
            blocage_compte(conn, username);
            system("clear");
            break;

        case 11:
            enregistrer_restitution(conn, username);
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
            system("clear");
            annuler_reservation_par_id(conn, username);
            system("clear");
            break;

        case 19:
            system("clear");
            afficher_reservations_utilisateur(conn, username);
            system("clear");
            break;

        case 20:
            system("clear");
            emprunter_livre_apres_reservation(conn, username);
            system("clear");
            break;

        case 21:
            valider_restitution_apres_transit(conn);
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