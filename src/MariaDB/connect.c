#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

void connect_database()
{
   // Variables pour la connexion à la base de données
   const char *user = "DB_USER";
   char *db_user = getenv(user);
   MYSQL *conn;

   // Initialise la connexion
   if (!(conn = mysql_init(0)))
   {
      fprintf(stderr, "Impossible d'initialiser la connexion\n");
      exit(1);
   }
   printf("Connexion initialsée !\n");

   // Mot de passe de l'utilisateur
   // const char *db_user_password = NULL;

   // Connexion à la base de données
   if (!mysql_real_connect(
           conn,         // Connexion
           "localhost",  // Hôte
           db_user,      // Nom d'utilisateur
           "",           // db_user_password,      // Mot de passe utilisateur
           "bibliotech", // Base de donnée par défaut
           3306,         // Numéro du port
           NULL,         // Chemin vers le fichier socket
           0             // Options supplémentaires
           ))
   {
      // Retourne l'échec de la connexion et ferme le gestionnaire
      fprintf(stderr, "Erreur de connexion au serveur: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }
}
