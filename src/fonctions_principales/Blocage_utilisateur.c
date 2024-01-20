#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qui_et_quand.h"
#include "gestion_entier.h"
#include "prototype_admin_general.h"
#include <mysql/mysql.h>
#include "fonctions_bdd.h"

#define ALLOWED_CHARS_FORMAT "[0-9a-zA-Z.@]" // format authorisé

int is_valid(const char *str)
{
    while (*str)
    {
        if (!strchr(ALLOWED_CHARS_FORMAT, *str))
        {
            return 0; // caractère non autorisé trouvé = is not valid
        }
        str++;
    }
    return 1; // tous les caractères sont valides = is valide
}

void Blocage_compte()
{

    int choix_type;     // variable pour type d'utilisateur
    char type_user[15]; // nom du type utilisateur choisi

    int user_group = get_user_type(getuid()); // groupe de l'utilisateur executant la commande

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 0: // admingeneral
        printf("Quel type d'utilisateur souhaitez-vous bloquer? [1]adherent/[2]adminsite/[3]admingeneral: ");
        do
        {
            printf("Le choix doit être compris entre 1 et 3\n");
            do
            {
                printf("Entrez votre choix : ");
                scanf("%d", &choix_type);
            } while (!gestion_int(choix_type));     // verification que c'est bien un entier et pas trop grand pour le buffer
        } while (choix_type < 1 || choix_type > 3); // verification de la valeur comprise
        break;
    case 1: // adminsite
        choix_type = 1;
        break;
    default:
        return;
        break;
    }
    // l'administrateur a choisi quel type d'utilisateur il souhaite supprimer

    switch (choix_type)
    {
    case 1: // inscrit
        type_user = "Inscrit";
        break;
    case 2: // adminsite
        type_user = "AdminSite";
        break;
    case 3: // admingeneral
        type_user = "AdminGeneral";
        break;
    }
    char *username, *time_str;
    qui_et_quand(&username, &time_str);
    // Ouvrez le fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "a");
    if (log_file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }

    // Écrivez l'en-tête du log
    fprintf(log_file, "Tentative de blocage par l'utilisateur: %s, Date et heure: %s\n", username, time_str);
    int mon_compteur_log = 5;
    do
    {
        mon_compteur_log--;
        printf("Veuillez saisir l'adresse mail complet de l'utilisateur que vous souhaitez bloquer : \n");
        scanf("%100s", login);
        if (mon_compteur_log == 0)
        {
            return
        }
    } while (!is_valid(login) && (mon_compteur_log >= 0));
    int mon_compteur_raison = 5;
    do
    {
        mon_compteur_raison--;
        printf("Veuillez saisir la raison dans un format autorisé (lettre, majuscule, chiffre, point, arobase) : \n");
        scanf("%100s", raison);
        if (mon_compteur_log == 0)
        {
            return
        }
    } while (!is_valid(login) && (mon_compteur_log >= 0));

    // Vérifiez si l'utilisateur existe
    char query_select[500];
    sprintf(query_select, "SELECT * FROM Utilisateur WHERE ID_Utilisateur = '%s'", login);
    if (mysql_query(conn, query_select))
    {
        fprintf(log_file, "Erreur lors de la vérification de l'utilisateur dans la base de données: %s\n", mysql_error(conn));
        fclose(log_file);
        free(time_str);
        return;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (mysql_num_rows(result) == 0)
    {
        printf("L'utilisateur avec le login '%s' n'a pas été trouvé dans la base de données.\n", login);
        fclose(log_file);
        free(time_str);
        return;
    }

    // Blocage de l'utilisateur de la base de données
    char query[500];
    sprintf(query, "UPDATE Utilisateur SET EstBloque = true, CommentaireBlocage = '%s' WHERE Email = '%s';", raison, login);
    if (mysql_query(conn, query))
    {
        fprintf(log_file, "Erreur de blocage dans la base de données: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Utilisateur bloqué avec succès de la base de données.\n");
    }

    // Suppression de l'utilisateur du système Ubuntu
    char command[500];
    sprintf(command, "sudo usermod -L %s", login);
    if (system(command) != 0)
    {
        fprintf(log_file, "Erreur lors de la suppression de l'utilisateur dans le système.\n");
    }
    else
    {
        printf("Utilisateur bloqué avec succès dans le système.\n");
        fprintf(log_file, "Utilisateur %s bloqué avec succès\n", login);
    }

    fclose(log_file);
    free(time_str);
}
