// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void choix_inscrit_bibliotheque(int user_groupe, char *username)
{
    int choix_user;

    printf("+-------------Bienvenue!!-------------+\n\n");
    printf("+---------Que souhaitez vous faire?----------+\n");
    printf("1) Lister les livres et leur disponibilité dans les différents sites\n");
    printf("2) Réserver un livre\n");
    printf("3) S'inscrire sur une liste attente pour réserver un livre\n");
    printf("4) Consulter le délai d attente pour emprunter un livre indisponible\n");
    printf("5) Déconnexion\n");

    printf("Veuillez entrer le numéro du choix correspondant : \n");
    scanf("%d", choix_user);

    switch (user_groupe)
    {
    case 1:
        Liste_livres_et_dispo();
        break;

    case 2:
        MYSQL *conn;
        Emprunt_soimeme(conn);
        break;

    case 3:
        Inscri_liste_attente_livre();
        break;

    case 4:
        Affichage_delai_attente_livre();
        break;

    default:
        // Fonction de déconnexion ou autre traitement
        break;
    }
}
