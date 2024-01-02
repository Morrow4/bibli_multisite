#include <mysql.h>

void connect(char* db_user)
{
   // Variable d'environnement de connection au compte utilisateur
   char *db_user = ("DB_USER");

   // Initialise la connection
   MYSQL *conn;
   if (!(conn = mysql_init(0)))
   {
      fprintf(stderr, "Impossible d'initialiser la connection\n");
      exit(1);
   }

   // Scanf
   char db_user_password;
   scanf("%d", &db_user_password);

   // Connection à la base de donnée
   if (!mysql_real_connect(
         conn,                 // Connection
         "mariadb.10.1.144.49",// Hôte
         "db_user",            // Compte utilisateur
         "db_user_password",   // Mot de passe utilisateur
         "bibliotech",         // Base de donnée par défault
         3306,                 // Numéro du port
         NULL,                 // Chemin au fichier socket
         0                     // Option additionnel
      ))
   {
      // Retourne l'échec de la connection et ferme le gestionnaire
      fprintf(stderr, "Erreur de connection au serveur: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }

}
