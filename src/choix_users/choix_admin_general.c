// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>

// Prototypes des fonctions
#include "prototype_admin_general.h"

void choix_admin_bibliotheque(int user_groupe) {

    printf("+------------------MENU-------------------+\n");
    printf("+---------Administrateur_Général----------+\n");
    switch(user_groupe) {
        case 1:
            printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
            Liste_livres_et_dispo();
            break;

        case 2:
            printf("2) Réserver un livre\n");
            Reservation_soimeme_Livre();
            break;

        case 3:
            printf("3) S inscrire sur une liste attente pour réserver un livre\n");
            Inscri_liste_attente_livre();
            break;

        case 4:
            printf("4) Consulter le délai d attente pour emprunter un livre indisponible\n");
            Affichage_delai_attente_livre();
            break;

        case 5:
            printf("5) Réserver un livre pour un utilisateur inscrit\n");
            Emprunt_pour_adherent();
            break;

        case 6:
            printf("6) Ajouter un compte utilisateur ou administrateur\n");
            Ajout_compte();
            break;

        case 7:
            printf("7) Supprimer un compte utilisateur ou administrateur\n");
            Suppression_compte();
            break;

        case 8:
            printf("8) Consulter les statistiques\n");
            Consultation_Stat();
            break;

        case 9:
            printf("9) Ajouter un livre\n");
            Ajout_livre();
            break;

        case 10:
            printf("10) Supprimer un livre\n");
            Suppression_livre();
            break;

        case 11:
            printf("11) Bloquer utilisateur temporairement\n");
            Fonction_blocage_utilisateur();
            break;

        case 12:
            printf("12) Valider la restitution un livre\n");
            validation_restitution();
            break;


        default:
            printf("Déconnexion\n");
            // Fonction de déconnexion ou autre traitement
            break;
    }
}
//test