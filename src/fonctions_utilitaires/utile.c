#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utilitaire.h"

int is_valid(char *allowed_format, const char *str)
{
    while (*str)
    {
        if (!strchr(*allowed_format, *str))
        {
            return 0; // caractère non autorisé trouvé = is not valid
        }
        str++;
    }
    return 1; // tous les caractères sont valides = is valid
}