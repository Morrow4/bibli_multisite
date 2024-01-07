#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <unistd.h>
#include "qui_et_quand.h"
#include "prototype_admin_general.h"

void Suppression_adherent(const char *login) {
    char *username, *time_str;
    qui_et_quand(&username, &time_str);
    // Ouvrez le fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }

    // Écrivez l'en-tête du log
    fprintf(log_file, "Utilisateur: %s, Date et heure: %s\n", username, time_str);

    // Vérifiez si l'utilisateur existe
    char query_select[500];
    sprintf(query_select, "SELECT * FROM Utilisateur WHERE ID_Utilisateur = '%s'", login);
    if (mysql_query(conn, query_select)) {
        fprintf(log_file, "Erreur lors de la vérification de l'utilisateur dans la base de données: %s\n", mysql_error(conn));
        fclose(log_file);
        free(time_str);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0) {
        printf("L'utilisateur avec le login '%s' n'a pas été trouvé dans la base de données.\n", login);
        fclose(log_file);
        free(time_str);
        return;
    }
    
    // Suppression de l'utilisateur de la base de données
    char query[500];
    sprintf(query, "DELETE FROM Utilisateur WHERE ID_Utilisateur = '%s'", login);
    if (mysql_query(conn, query)) {
        fprintf(log_file, "Erreur de suppression dans la base de données: %s\n", mysql_error(conn));
    } else {
        printf("Utilisateur supprimé avec succès de la base de données.\n");
    }

    // Suppression de l'utilisateur du système Ubuntu
    char command[500];
    sprintf(command, "userdel -r %s", login);
    if (system(command) != 0) {
        fprintf(log_file, "Erreur lors de la suppression de l'utilisateur du système.\n");
    } else {
        printf("Utilisateur supprimé avec succès du système.\n");
    }
    
    fclose(log_file);
    free(time_str);
}
