// Inclusion des en-têtes nécessaires
#include <stdio.h>
#include <stdlib.h>

// Prototypes des fonctions (à définir dans un autre fichier)
#include "utilitaire.h"
#include "fonctions_bdd.h"

void choix_admin_site_bibliotheque(int user_groupe)
{
    printf("+------------------MENU-------------------+\n");
    printf("+-----------Administrateur_Site-----------+\n");
    switch (user_groupe)
    {
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
        printf("6) Ajouter un compte utilisateur\n");
        Ajout_compte();
        break;

    case 7:
        printf("7) Supprimer un compte utilisateur\n");
        Suppression_compte();
        break;

    case 8:
        printf("8) Consulter les statistiques\n");
        Consultation_Stat();
        break;

    case 9:
        printf("9) Ajouter un livre\n");
        MYSQL* conn;
        Ajout_livre(conn);
        break;

    case 10:
        printf("10) Supprimer un livre\n");
        Suppression_livre();
        break;

    case 11:
        printf("11) Bloquer utilisateur temporairement\n");
        Blocage_utilisateur();
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
