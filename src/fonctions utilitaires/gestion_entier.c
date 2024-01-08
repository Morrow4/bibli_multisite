#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool estEntier(const char *str) {
    char *c;

    if (!str || *str == '\0') {
        return false;
    }

    c=str;

    while ( *c != 0 ) {
        if (*c < '0' || *c > '9') {
            return false;
        }
        c++;
    }
    return true;
}

bool limiteTailleInt(const char *str, int limite) {
    if (!str || *str == '\0') {
        return false;
    }

    if (strlen(str) > limite) {
        return false;
    }

    return estEntier(str);
}

bool gestion_int(int valeur) {
    char str[11]; // une taille suffisamment grande pour stocker un int
    snprintf(str, sizeof(str), "%d", valeur); // convertit l'entier en chaîne de caractères

    if (!estEntier(str)) {
        printf("La valeur n'est pas un entier.\n");
        return false;
    }

    if (!limiteTailleInt(str, 9)) {
        printf("Nombre incorrect.\n");
        return false;
    }

    return true;
}

/* exemple utilisation:
int main() {
    int valeur;
    printf("Entrez une valeur : ");
    scanf("%d", &valeur);

    if (gestion_int(valeur)) {
        printf("L'entier et sa taille sont valides.\n");
    } else {
        printf("L'entier ou sa taille est invalide.\n");
    }

    return 0;
}
*/

