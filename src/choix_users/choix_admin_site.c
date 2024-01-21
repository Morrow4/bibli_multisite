// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_admin_site_bibliotheque(MYSQL *conn, char *username)
{
    int choix_user;

    printf("+-------------------------------MENU-------------------------------+\n");
    printf("+------------------------Administrateur Site-----------------------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("2) Réserver un livre\n");
    printf("3) S inscrire sur une liste attente pour réserver un livre\n");
    printf("4) Consulter le délai d attente pour emprunter un livre indisponible\n");
    printf("5) Réserver un livre pour un utilisateur inscrit\n");
    printf("6) Ajouter un compte utilisateur\n");
    printf("7) Supprimer un compte utilisateur\n");
    printf("8) Consulter les statistiques\n");
    printf("9) Ajouter un livre\n");
    printf("10) Supprimer un livre\n");
    printf("11) Bloquer utilisateur temporairement\n");
    printf("12) Valider la restitution un livre\n");
    printf("13) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", &choix_user);

    switch (choix_user)
    {
    case 1:
        Liste_livres_et_dispo(conn);
        break;

    case 2:
        Emprunt_soimeme(conn, username);
        break;

    case 3:
        Inscri_liste_attente_livre();
        break;

    case 4:
        Affichage_delai_attente_livre();
        break;

    case 5:
        Emprunt_pour_adherent();
        break;

    case 6:
        ajout_compte(conn);
        break;

    case 7:
        suppression_compte(conn);
        break;

    case 8:
        Consultation_Stat();
        break;

    case 9:
        ajout_livre(conn);
        break;

    case 10:
        suppression_livre(conn);
        break;

    case 11:
        blocage_compte(conn);
        break;

    case 12:
        valider_restitution(conn);
        break;

    default:
        // Fonction de déconnexion ou autre traitement
        break;
    }
}
