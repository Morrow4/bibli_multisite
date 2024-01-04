// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>

// Prototypes des fonctions (à définir dans un autre fichier)
#include prototype_admin_site.h

void choix_admin_bibliotheque(int user_groupe) {
    switch(user_groupe) {
        case 1:
            printf("Lister les livres et leur disponibilité dans les différents sites\n");
            Liste_livres_et_dispo();
            break;

        case 2:
            printf("Réserver un livre\n");
            Reservation_soimeme_Livre();
            break;

        case 3:
            printf("S inscrire sur une liste attente pour réserver un livre\n");
            Inscri_liste_attente_livre();
            break;

        case 4:
            printf("Consulter le délai d attente pour emprunter un livre indisponible\n");
            Affichage_delai_attente_livre();
            break;

        case 5:
            printf("Réserver un livre pour un utilisateur inscrit\n");
            Reservation_pour_adherent();
            break;

        case 6:
            printf("Ajouter un compte utilisateur\n");
            Ajout_adherent();
            break;

        case 7:
            printf("Supprimer un compte utilisateur\n");
            Suppression_adherent(login);
            break;

        case 8:
            printf("Consulter les statistiques\n");
            Consultation_Stat();
            break;

        case 9:
            printf("Ajouter un livre\n");
            Ajout_livre();
            break;

        case 10:
            printf("Supprimer un livre\n");
            Suppression_livre();
            break;

        case 11:
            printf("Bloquer utilisateur temporairement\n");
            Blocage_utilisateur();
            break;

        case 12:
            printf("Valider la restitution un livre\n");
            validation_restitution();
            break;

        default:
            printf("Déconnexion\n");
            // Fonction de déconnexion ou autre traitement
            break;
    }
}
