#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "../header/utilitaire.h"
#include "../header/fonctions_bdd.h"
#include "../header/fonctions_choix_user.h"

MYSQL *connect_database()
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

   // Définir le délai d'attente de lecture (net_read_timeout) en secondes
   unsigned int read_timeout = 60;
   mysql_options(conn, MYSQL_OPT_READ_TIMEOUT, &read_timeout);

   // Mot de passe de l'utilisateur
   // const char *db_user_password = NULL;

   // Connexion à la base de données
   if (!conn = mysql_real_connect(
           conn,
           "localhost",
           db_user,
           "",
           "bibliotech",
           3306,
           NULL,
           0))
   {
      // Retourne l'échec de la connexion
      fprintf(stderr, "Erreur de connexion au serveur: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }

   return conn;
}
