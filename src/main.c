#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    // Ouvrir la connexion à la base de données
    connect_database.c;
    //Recupération du groupe (BD ou systeme au choix)
    //Renvoyer vers les fonctions par utilisateur
    switch(user_groupe)
    {
        case admingeneral:
            choix_admin_general.c;
            break;

        case adminsite:
            choix_admin_site.c;
            break;

        case inscrit:
            choix_inscrit.c;
            break;

        case invite:
            choix_invite.c;
            break;

        default:
            printf("Déconnexion\n");
            // Fonction de déconnexion ou autre traitement
            break;       
    }
    // Fermer la connexion à la base de données lorsque vous avez fini de l'utiliser
    mysql_close(conn);

    return 0;
}