#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <mysql/mysql.h>
#include "header/utilitaire.h"
#include "header/fonctions_choix_user.h"
#include "header/fonctions_bdd.h"

int main()
{
    uid_t user_uid = getuid();

    // Ouvrir la connexion à la base de données
    MYSQL* conn = connect_database();

    // On utilise la fonction getpwuid() pour récupérer les informations sur l'utilisateur
    struct passwd *pwd = getpwuid(user_uid);

    // Vérifiez si les informations sur l'utilisateur ont été trouvées
    if (pwd == NULL)
    {
        // Gestion d'erreur si les informations ne sont pas disponibles
        fprintf(stderr, "Impossible de récupérer les informations sur l'utilisateur.\n");

        return 1; // Quitter le programme avec un code d'erreur
    }
    // Récupérez le nom d'utilisateur (login) à partir de la structure passwd
    char *username = pwd->pw_name;

    printf("username : %s\n", username);

    // Recupération du groupe
    int user_group = get_user_type(conn, username);

    printf("user group : %d\n", user_group);

    // Renvoyer vers les fonctions par utilisateur
    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        choix_admin_general_bibliotheque(conn, username);
        break;

    case 2: // adminsite
        choix_admin_site_bibliotheque(conn, username);
        break;

    case 3: // inscrit
        choix_inscrit_bibliotheque(conn, username);
        break;

    case 4: // invite
        choix_invite_bibliotheque(conn);
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