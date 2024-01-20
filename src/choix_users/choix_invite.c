// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>

// Prototypes des fonctions (à définir dans un autre fichier)
#include "prototype_invite.h"

void choix_invite_bibliotheque(int user_groupe) {
    printf("+-------------Bienvenu!!-------------+\n\n");
    printf("Que voulez-vous faire?\n");
    switch(user_groupe) {
        case 1:
            printf("Voici la liste des livres et leur disponibilité dans les différents sites\n");
            Liste_livres_et_dispo();
            break;

        default:
            printf("Déconnexion\n");
            // Fonction de déconnexion ou autre traitement
            break;
    }
}
