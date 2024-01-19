#include <pwd.h>
#include <time.h>
#include <unistd.h>

#include "qui_et_quand.h" //son prototype

void qui_et_quand(char **username, char **time_str) {
    // Obtenir le nom de l'utilisateur à l'origine de l'exécution
    qui(username);

    // Obtenir la date et l'heure actuelles
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    *time_str = malloc(64);
    strftime(*time_str, 64, "%Y-%m-%d %H:%M:%S", local_time);
}
