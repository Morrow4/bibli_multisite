// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_invite_bibliotheque(int user_groupe)
{
    int choix_user;

    printf("+-------------Bienvenue!!-------------+\n\n");
    printf("Que voulez-vous faire?\n");
    printf("1) Afficher la liste des livres et leur disponibilité\n");
    printf("2) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", choix_user);

    switch (user_groupe)
    {
    case 1:
        printf("Voici la liste des livres et leur disponibilité dans les différents sites\n");
        Liste_livres_et_dispo();
        break;

    default:
        // Fonction de déconnexion ou autre traitement
        break;
    }
}
