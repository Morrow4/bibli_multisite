// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_admin_general_bibliotheque(int user_groupe, char *username)
{
    int choix_user;

    printf("+------------------MENU-------------------+\n");
    printf("+---------Administrateur_Général----------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("2) Réserver un livre\n");
    printf("3) S inscrire sur une liste attente pour réserver un livre\n");
    printf("4) Consulter le délai d attente pour emprunter un livre indisponible\n");
    printf("5) Réserver un livre pour un utilisateur inscrit\n");
    printf("6) Ajouter un compte utilisateur ou administrateur\n");
    printf("7) Supprimer un compte utilisateur ou administrateur\n");
    printf("8) Consulter les statistiques\n");
    printf("9) Ajouter un livre\n");
    printf("10) Supprimer un livre\n");
    printf("11) Bloquer utilisateur temporairement\n");
    printf("12) Valider la restitution un livre\n");
    printf("13) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", choix_user);

    switch (user_groupe)
    {
    case 1:
        Liste_livres_et_dispo();
        break;

    case 2:
        Reservation_soimeme_Livre();
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
        Ajout_compte();
        break;

    case 7:
        Suppression_compte();
        break;

    case 8:
        Consultation_Stat();
        break;

    case 9:
        Ajout_livre();
        break;

    case 10:
        Suppression_livre();
        break;

    case 11:
        Fonction_blocage_utilisateur();
        break;

    case 12:
        validation_restitution();
        break;

    default:
        // Fonction de déconnexion ou autre traitement
        break;
    }
}