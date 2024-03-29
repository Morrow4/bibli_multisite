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

    while (choix_user != 20)
    {
        printf("+----------------------------------------------------------------+\n");
        printf("|                         Menu Principal                         |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("|                       Gestion des Livres                       |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 1)  Lister les livres et leur disponibilité dans les sites     |\n");
        printf("| 2)  Ajouter un livre                                           |\n");
        printf("| 3)  Mettre à jour les informations d'un livre                  |\n");
        printf("| 4)  Supprimer un livre                                         |\n");
        printf("| 5)  Ajouter un exemplaire                                      |\n");
        printf("| 6)  Mettre à jour les informations d'un exemplaire             |\n");
        printf("| 7)  Supprimer un exemplaire                                    |\n");
        printf("| 8)  Enregistrer la restitution d'un livre                      |\n");
        printf("| 9)  Valider la restitution des livres rappatriés               |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|                    Gestion des Utilisateurs                    |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 10) Ajouter un compte utilisateur ou administrateur            |\n");
        printf("| 11) Supprimer un compte utilisateur ou administrateur          |\n");
        printf("| 12) Bloquer un utilisateur                                     |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|            Gestion des Réservations et des Emprunts            |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 13) Réserver un livre pour un utilisateur inscrit              |\n");
        printf("| 14) Emprunter un livre pour un utilisateur inscrit             |\n");
        printf("| 15) Annuler une réservation                                    |\n");
        printf("| 16) Voir mes réservations                                      |\n");
        printf("| 17) Voir mes emprunts                                          |\n");
        printf("| 18) Emprunter un livre après réservation                       |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|                          Statistiques                          |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 19) Consulter les statistiques globales de la bibliothèque     |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|                         Autres Options                         |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 20) Déconnexion                                                |\n");
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

        case 2:
            system("clear");
            ajout_livre(conn);
            break;

        case 3:
            system("clear");
            mise_a_jour_livre(conn);
            break;

        case 4:
            system("clear");
            suppression_livre(conn);
            break;

        case 5:
            system("clear");
            ajout_exemplaire(conn);
            break;

        case 6:
            system("clear");
            mise_a_jour_exemplaire(conn);
            break;

        case 7:
            system("clear");
            suppression_exemplaire(conn);
            break;

        case 8:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre : ");
            scanf("%s", login_utilisateur);
            enregistrer_restitution(conn, login_utilisateur);
            break;

        case 9:
            system("clear");
            valider_restitution_apres_transit(conn);
            break;

        case 10:
            system("clear");
            ajout_compte(conn, username);
            break;

        case 11:
            system("clear");
            suppression_compte(conn, username);
            break;

        case 12:
            system("clear");
            blocage_compte(conn, username);
            break;

        case 13:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur qui veut réserver un livre : ");
            scanf("%s", login_utilisateur);
            reserver_livre(conn, login_utilisateur);
            break;

        case 14:
            printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre : ");
            scanf("%s", login_utilisateur);
            emprunter_livre(conn, login_utilisateur);
            // system("clear");
            break;

        case 15:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur qui veut annuler un emprunt : ");
            scanf("%s", login_utilisateur);
            annuler_reservation_par_id(conn, login_utilisateur);
            break;

        case 16:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur dont vous voulez afficher les réservations : ");
            scanf("%s", login_utilisateur);
            afficher_reservations_utilisateur(conn, login_utilisateur);
            break;

        case 17:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur dont vous voulez afficher les emprunts : ");
            scanf("%s", login_utilisateur);
            afficher_emprunts_non_restitues_utilisateur(conn, login_utilisateur);
            break;

        case 18:
            system("clear");
            printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre réservé : ");
            scanf("%s", login_utilisateur);
            emprunter_livre_apres_reservation(conn, login_utilisateur);
            break;

        case 19:
            system("clear");
            consultation_stat(conn, user_type);
            break;

        case 20:
            system("clear");
            deconnexion(conn);
            break;

        default:
            printf("\n+-----------------------------------+\n");
            printf("+ Choix invalide. Veuillez réessayer.+\n");
            printf("+-----------------------------------+\n\n");
            system("clear");
            break;
        }
    }
}