// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_admin_general_bibliotheque(MYSQL *conn, char *username)
{
    int choix_user;
    int ISBN;
    int id_emprunt;
    char *login_utilisateur == NULL;

    printf("+-------------------------------MENU-------------------------------+\n");
    printf("+----------------------Administrateur Général----------------------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("2) Réserver un livre\n");
    printf("3) Réserver un livre pour un utilisateur inscrit\n");
    printf("4) Ajouter un compte utilisateur ou administrateur\n");
    printf("5) Supprimer un compte utilisateur ou administrateur\n");
    printf("6) Consulter les statistiques\n");
    printf("7) Ajouter un livre\n");
    printf("8) Supprimer un livre\n");
    printf("9) Bloquer un utilisateur temporairement\n");
    printf("10) Valider la restitution d'un livre\n");
    printf("11) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", &choix_user);

    switch (choix_user)
    {
    case 1:
        // Liste_livres_et_dispo();
        break;

    case 2:
        printf("Veuillez saisir l'ISBN du livre que vous voulez réserver : ");
        scanf("%d", &ISBN);
        reserver_livre(conn, username, ISBN);
        free(ISBN);
        break;

    case 3:
        printf("Veuillez saisir le login de l'utilisateur qui veut réserver un livre : ");
        scanf("%d", &login_utilisateur);
        printf("Veuillez saisir l'ISBN du livre que vous voulez réserver : ");
        scanf("%d", &ISBN);
        reserver_livre(conn, login_utilisateur, ISBN);
        free(login_utilisateur);
        free(ISBN);
        break;

    case 4:
        ajout_compte(conn);
        break;

    case 5:
        Suppression_compte(conn);
        break;

    case 6:
        // Consultation_Stat();
        break;

    case 7:
        ajout_livre(conn);
        break;

    case 8:
        printf("Veuillez saisir l'ISBN du livre que vous voulez supprimer : ");
        scanf("%d", &ISBN);
        suppression_livre(conn, &ISBN);
        break;

    case 9:
        Blocage_compte(conn);
        break;

    case 10:
        verifier_et_valider_restitution(conn,id_emprunt);
        break;

    default:
        // Fonction de déconnexion ou autre traitement
        break;
    }
}