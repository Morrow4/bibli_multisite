#include <mysql.h>

void connect(int argc, char* argv[])
{

   // initialise la connection
   MYSQL *conn;
   if (!(conn = mysql_init(0)))
   {
      fprintf(stderr, "Impossible d'initialiser la connection\n");
      exit(1);
   }

   // Connect to the database
   if (!mysql_real_connect(
         conn,                 // Connection
         "10.1.144.49",        // Host
         "db_user",            // User account
         "db_user_password",   // User password
         "bibliotech",         // Default database
         3306,                 // Port number
         NULL,                 // Path to socket file
         0                     // Additional options
      ))
   {
      // retourne l'échec de la connection et ferme le gestionnaire
      fprintf(stderr, "Erreur de connection au serveur: %s\n", mysql_error(conn));
      mysql_close(conn);
      exit(1);
   }

   // Utilise la connection
   // ...

   // Ferme la connection
   mysql_close(conn);

}
