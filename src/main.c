#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main() {
    uid_t user_uid = getuid();

    // Ouvrir la connexion à la base de données
    connect_database();

    //Recupération du groupe (BD ou systeme au choix)
    int user_group = get_user_type(user_uid);

    //Renvoyer vers les fonctions par utilisateur
    switch(user_group) //pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
        case 1: //admingeneral
            choix_admin_general();
            break;

        case 2: //adminsite
            choix_admin_site();
            break;

        case 3: //inscrit
            choix_inscrit();
            break;

        case 4: //invite
            choix_invite();
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