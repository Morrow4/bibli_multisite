#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_admin_general_bibliotheque(MYSQL *conn, char *username)
{
    int choix_user;
    char ISBN[13];
    int id_emprunt = -1;
    char login_utilisateur[100];
    char site_restitution[50];
    char id_exemplaire[100];

    while (choix_user != 17) // L'option de déconnexion est le choix 17
    {
        printf("\n+-------------------------------MENU-------------------------------+\n");
        printf("+----------------------Administrateur Général----------------------+\n");
        printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
        printf("2) Réserver un livre pour soi\n");
        printf("3) Réserver un livre pour un utilisateur inscrit\n");
        printf("4) Ajouter un compte utilisateur ou administrateur\n");
        printf("5) Supprimer un compte utilisateur ou administrateur\n");
        printf("6) Consulter les statistiques globales de la bibliothèque\n");
        printf("7) Ajouter un livre\n");
        printf("8) Mettre à jour les informations d'un livre\n");
        printf("9) Supprimer un livre\n");
        printf("10) Bloquer un utilisateur temporairement\n");
        printf("11) Valider la restitution d'un livre\n");
        printf("12) Emprunter un livre pour soi\n");
        printf("13) Emprunter un livre pour un utilisateur inscrit\n");
        printf("14) Ajouter un exemplaire\n");
        printf("15) Mettre à jour les informations d'un exemplaire\n");
        printf("16) Supprimer un exemplaire\n");
        printf("17) Déconnexion\n");

        printf("\nVeuillez entrer le numéro du choix correspondant : ");
        scanf("%d", &choix_user);
        printf("\n\n");

        switch (choix_user)
        {
        case 1:
            afficher_tous_les_livres(conn);
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
            printf("Veuillez saisir l'ISBN du livre que vous voulez mettre à jour : ");
            scanf("%s", ISBN);
            mise_a_jour_livre(conn, ISBN);
            break;

        case 9:
            printf("Veuillez saisir l'ISBN du livre que vous voulez supprimer : ");
            scanf("%s", ISBN);
            suppression_livre(conn, ISBN);
            break;

        case 10:
            blocage_compte(conn, username);
            break;

        case 11:
            printf("Veuillez saisir le numéro d'identification de l'emprunt à restituer : ");
            scanf("%d", &id_emprunt);
            printf("Veuillez saisir le site sur lequel le livre est restitué : ");
            scanf("%s", site_restitution);
            enregistrer_restitution(conn, id_emprunt, site_restitution);
            break;

        case 12:
            Emprunt_soimeme(conn, username);
            break;

        case 13:
            printf("Veuillez saisir le login de l'utilisateur qui veut emprunter un livre : ");
            scanf("%s", login_utilisateur);
            Emprunt_soimeme(conn, login_utilisateur);
            break;

        case 14:
            ajout_exemplaire(conn);
            break;

        case 15:
            printf("Veuillez saisir le numéro d'identification de l'exemplaire que vous voulez mettre à jour : ");
            scanf("%s", id_exemplaire);
            mise_a_jour_exemplaire(conn, id_exemplaire);
            break;

        case 16:
            printf("Veuillez saisir le numéro d'identification de l'exemplaire que vous voulez supprimer : ");
            scanf("%s", id_exemplaire);
            suppression_livre(conn, id_exemplaire);
            break;

        case 17:
            deconnexion(conn);
            break;

        default:
            printf("\n/===================================/\n");
            printf("/Choix invalide. Veuillez réessayer./\n");
            printf("/===================================/\n\n");
            break;
        }
    }
}