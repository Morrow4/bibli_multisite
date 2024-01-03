// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>

// Prototypes des fonctions (à définir dans un autre fichier)
#include prototype_inscrit.h

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

        default:
            printf("Déconnexion\n");
            // Fonction de déconnexion ou autre traitement
            break;
    }
}
