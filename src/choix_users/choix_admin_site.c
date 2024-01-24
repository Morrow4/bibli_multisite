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
    int id_emprunt = -1;
    char login_utilisateur[100];
    char site_restitution[50];
    int user_type = 1;

    printf("                      .--.   _\n");
    printf("                  .---|__| .((\\=.\n");
    printf("               .--|===|--|/    ,(\n");
    printf("               |  |===|  |\\      y\n");
    printf("               |--|   |  | `.__,'\n");
    printf("               |--|   |  | /  \\\\\n");
    printf("               |  |   |  |/|  | \\`----.\n");
    printf("               |  |   |  ||\\  \\  |___.'_\n");
    printf("              _|  |   |__||,\\  \\-+-._.' )_\n");
    printf("             / |  |===|--|\\  \\  \\      /  \\\n");
    printf("            /  `--^---'--' `--`-'---^-'    \\\n");
    printf("              '================================`\n");
    while (choix_user != 19) // L'option de déconnexion est le choix 17
    {
        printf("+----------------------------------------------------------------+\n");
        printf("|                      Menu Principal                             |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("|                        Gestion des Livres                      |\n");
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
        printf("|                        Gestion des Utilisateurs                |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 10) Ajouter un compte utilisateur                              |\n");
        printf("| 11) Supprimer un compte utilisateur                            |\n");
        printf("| 12) Bloquer           un utilisateur                           |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|                        Gestion des Réservations                |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 13) Réserver un livre pour un utilisateur inscrit              |\n");
        printf("| 14) Emprunter un livre pour un utilisateur inscrit             |\n");
        printf("| 15) Annuler une réservation                                    |\n");
        printf("| 16) Voir mes réservations                                      |\n");
        printf("| 17) Emprunter un livre après réservation                       |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|                        Statistiques                            |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 18) Consulter les statistiques du site de la bibliothèque      |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("|                        Autres Options                          |\n");
        printf("|----------------------------------------------------------------|\n");
        printf("| 19) Déconnexion                                                |\n");
        printf("+----------------------------------------------------------------+\n");
        printf("\nVeuillez entrer le numéro du choix correspondant : \n");
        scanf("%d", &choix_user);

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
            enregistrer_restitution(conn, username);
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
            reserver_livre(conn, login_utilisateur);
            break;

        case 14:
            printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre : ");
            scanf("%s", login_utilisateur);
            Emprunt_soimeme(conn, login_utilisateur);
            // system("clear");
            break;

        case 15:
            system("clear");
            annuler_reservation_par_id(conn, username);
            break;

        case 16:
            system("clear");
            afficher_reservations_utilisateur(conn, username);
            break;

        case 17:
            system("clear");
            emprunter_livre_apres_reservation(conn, username);
            break;

        case 18:
            system("clear");
            consultation_stat(conn, user_type);
            break;

        case 19:
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
