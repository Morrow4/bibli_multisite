#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

   // Définir le délai d'attente de lecture (net_read_timeout) en secondes
   unsigned int read_timeout = 60;
   mysql_options(conn, MYSQL_OPT_READ_TIMEOUT, &read_timeout);

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

   char q[255];
   sprintf(q, "INSERT INTO Livre (ISBN, Titre, Auteur, Edition, Genre) VALUES ('test', 'test', 'test', 'test', 'test')");

   // Exécuter la requête SQL
   if (mysql_query(conn, q))
   {
      fprintf(stderr, "Erreur lors de l ajout du livre : %s\n", mysql_error(conn));
      return;
   }

   printf("Succes de l ajout du livre !\n");
}
