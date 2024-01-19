#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include "utilitaire.h"
#include "livre.h"

int main()
{
    uid_t user_uid = getuid();

    // Ouvrir la connexion à la base de données
    connect_database();

    // Utilisez la fonction getpwuid() pour récupérer les informations sur l'utilisateur
    struct passwd *pwd = getpwuid(uid);

    // Vérifiez si les informations sur l'utilisateur ont été trouvées
    if (pwd != NULL)
    {
        // Récupérez le nom d'utilisateur (login) à partir de la structure passwd
        char *username = pwd->pw_name;
    }
    else
    {
        // Gestion d'erreur si les informations ne sont pas disponibles
        fprintf(stderr, "Impossible de récupérer les informations sur l'utilisateur.\n");

        return 1; // Quitter le programme avec un code d'erreur
    }

    // Recupération du groupe (BD ou systeme au choix)
    int user_group = get_user_type(conn, username);

    // Renvoyer vers les fonctions par utilisateur
    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        choix_admin_general();
        break;

    case 2: // adminsite
        choix_admin_site();
        break;

    case 3: // inscrit
        choix_inscrit();
        break;

    case 4: // invite
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