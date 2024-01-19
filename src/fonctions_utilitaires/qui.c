#include <pwd.h>
#include <time.h>
#include <unistd.h>

#include "qui.h" //son prototype

void qui(char **username) {
    // Obtenir le nom de l'utilisateur à l'origine de l'exécution
    uid_t uid = getuid();
    struct passwd *pwd = getuid(uid);
    *username = (pwd != NULL) ? pwd->pw_name : "Inconnu";
}
