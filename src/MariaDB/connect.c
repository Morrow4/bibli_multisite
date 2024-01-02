#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

void connect_database()
{
   // Variables pour la connexion à la base de données
   const char *user = "DB_USER";
   char *db_user = getenv(db_user);
   MYSQL *conn;

   // Initialise la connexion
   if (!(conn = mysql_init(0)))
   {
      fprintf(stderr, "Impossible d'initialiser la connection\n");
      exit(1);
   }

   // Mot de passe de l'utilisateur
   char db_user_password[63]; // Utilisation d'un tableau de caractère pour stocker le mot de passe
   pritnf("Veuillez entrer le mot de passe de l'utilisateur :\n ")
   scanf("%62s", db_user_password); // Utilisation de "%s" pour les chaînes de caractères

   // Connexion à la base de données
   if (!mysql_real_connect(
         conn,                  // Connection
         "mariadb.10.1.144.49", // Hôte
         db_user,               // Nom d'utilisateur
         db_user_password,      // Mot de passe utilisateur
         "bibliotech",          // Base de donnée par défaut
         3306,                  // Numéro du port
         NULL,                  // Chemin vers le fichier socket
         0                      // Options supplémentaires
      ))
   {
      // Retourne l'échec de la connexion et ferme le gestionnaire
      fprintf(stderr, "Erreur de connection au serveur: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }

}
