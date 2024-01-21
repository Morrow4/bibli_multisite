#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

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

void ajout_compte(MYSQL *conn, char *username)
{
    int choix_type;     // variable pour type d'utilisateur
    char type_user[15]; // nom du type utilisateur choisi

    int user_group = get_user_type(conn, username); // groupe de l'utilisateur executant la commande

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 0: // admingeneral
        printf("Quel type d'utilisateur souhaitez-vous ajouter? [1]adherent/[2]adminsite/[3]admingeneral: ");
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
        break;
    }
    // Demander à l'administrateur quel type d'utilisateur il souhaite ajouter

    switch (choix_type)
    {
    case 1: // inscrit
        strcpy(type_user, "Inscrit");
        break;
    case 2: // adminsite
        strcpy(type_user, "AdminSite");
        break;
    case 3: // admingeneral
        strcpy(type_user, "AdminGeneral");
        break;
    }
    char login[101], password[256], nom[51], prenom[51], email[101], estChercheur[2], info_valid[2];
    // Alerte utilisateur
    printf("\nles caractères speciaux ne pourront pas être utilisé\n");
    printf("En cas d'erreur de saisi, veuillez remplir le formulaire dans son integralité, une validation sera demandée en fin de saisie\\n");
    do
    {
        // Saisie formulaire utilisateur
        int mon_compteur_login = 5;
        do
        {
            mon_compteur_login--;
            printf("Entrez le login : ");
            scanf("%100s", login);
            if (mon_compteur_login == 0)
            {
                return;
            }
        } while (!is_valid(login) && (mon_compteur_login >= 0));
        int mon_compteur_mp = 5;
        do
        {
            mon_compteur_mp--;
            printf("Entrez le mot de passe : ");
            scanf("%255s", password);
            if (mon_compteur_mp == 0)
            {
                return;
            }
        } while (!is_valid(password) && (mon_compteur_mp >= 0));
        int mon_compteur_mail = 5;
        do
        {
            mon_compteur_mail--;
            printf("Entrez le mail : ");
            scanf("%100s", email);
            if (mon_compteur_mail == 0)
            {
                return;
            }
        } while (!is_valid(email) && (mon_compteur_mail >= 0));
        int mon_compteur_nom = 5;
        do
        {
            mon_compteur_nom--;
            printf("Entrez le nom : ");
            scanf("%50s", nom);
            if (mon_compteur_nom == 0)
            {
                return;
            }
        } while (!is_valid(nom) && (mon_compteur_nom >= 0));
        int mon_compteur_prenom = 5;
        do
        {
            mon_compteur_prenom--;
            printf("Entrez le prenom : ");
            scanf("%50s", prenom);
            if (mon_compteur_prenom == 0)
            {
                return;
            }
        } while (!is_valid(prenom) && (mon_compteur_prenom >= 0));
        int mon_compteur_cherch = 5;
        do
        {
            mon_compteur_cherch--;
            printf("Est-ce un chercheur ? (oui/non) : ");
            scanf("%1s", estChercheur);
            if (mon_compteur_cherch == 0)
            {
                return;
            }
        } while ((strcmp(estChercheur, "o") != 0 && strcmp(estChercheur, "n") != 0) && (mon_compteur_cherch >= 0));

        printf("Le login est %s, le mot de passe est %s, son groupe est %s, le mail est %s, le nom est %s, le prenom est %s, la personne est chercheur : %s", login, password, type_user, email, nom, prenom, estChercheur);
        do
        {
            printf("Validez-vous ces informations? o/n");
            scanf("%1s", info_valid);
        } while (strcmp(info_valid, "o") != 0 && strcmp(info_valid, "n") != 0);
    } while (strcmp(info_valid, "n"));

    // Ouverture du fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "r");
    if (log_file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }

    char date_et_heure[50] = ctime(time_str);
    fprintf(log_file, "Exécuté par: %s, Date et heure: %s\n", username, date_et_heure);

    //  Ajout de l'utilisateur dans la base de donnée //Modifier pour mail
    char query[1024];
    sprintf(query, "INSERT INTO Utilisateur (ID_Utilisateur, Nom, Prenom, MotDePasse, TypeUtilisateur, EstChercheur) VALUES ('%s','%s', '%s', '%s', '%s', '%s')",
            login, nom, prenom, password, type_user, (strcmp(estChercheur, "o") == 0) ? "1" : "0");

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur d'insertion: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Utilisateur ajouté avec succès à la base de données.\n");
    }
    fprintf(log_file, "Utilisateur créé dans la base de donnée: %s, Groupe: %s\n", login, type_user); // log ajout bdd
    // Création de l'utilisateur sur le système
    char user_password[256];
    strcpy(username, login);
    strcpy(user_password, password);

    // Exécutez la commande pour ajouter l'utilisateur
    char command[500];
    sprintf(command, "useradd -m -d /home/bibliotech/%s/ -s /home/bibliotech/%s/demarrage_app.sh -G %s %s", type_user, type_user, type_user, username);

    // Vérification du succès de l'exécution de la commande
    if (system(command) != 0)
    {
        fprintf(stderr, "Erreur lors de l'ajout de l'utilisateur sur le système.\n");
        fprintf(log_file, "Erreur lors de l'ajout de l'utilisateur sur le système.\n"); // log erreur sys
        exit(1);
    }
    fprintf(log_file, "Utilisateur créé dans le systeme: %s, Groupe: %s\n", login, type_user); // log ajout sys
    // Ajout du mot de passe utilisateur
    system(command);

    char passwd[500];
    sprintf(passwd, "echo \"%s:%s\" | chpasswd", username, user_password);
    system(passwd);                                                                                  // Cela utilise chpasswd pour définir le mot de passe de l'utilisateur
    fprintf(log_file, "Mot de passe de l'utilisateur attribué: %s, Groupe: %s\n", login, type_user); // log mp sys

    // Fermetures
    fclose(log_file);
    free(time_str);
}

void suppression_compte(MYSQL *conn, char *username)
{

    int choix_type; // variable pour type d'utilisateur
    char login[101];
    char type_user[15]; // nom du type d'utilisateur

    int user_group = get_user_type(conn, username); // groupe de l'utilisateur executant la commande

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 0: // admingeneral
        printf("Quel type d'utilisateur souhaitez-vous supprimer? [1]adherent/[2]adminsite/[3]admingeneral: ");
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
        strcpy(type_user, "Inscrit");
        break;
    case 2: // adminsite
        strcpy(type_user, "AdminSite");
        break;
    case 3: // admingeneral
        strcpy(type_user, "AdminGeneral");
        break;
    }

    // Ouvrez le fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "a");
    if (log_file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }

    char *time_str;
    qui_et_quand(&username, &time_str);

    // Écrivez l'en-tête du log
    fprintf(log_file, "Tentative de suppression par l'utilisateur: %s, Date et heure: %s\n", username, time_str);
    int mon_compteur_log = 5;
    do
    {
        mon_compteur_log--;
        printf("Veuillez saisir l'adresse mail complet de l'utilisateur que vous souhaitez supprimer : \n");
        scanf("%100s", login);
        if (mon_compteur_log == 0)
        {
            fclose(log_file); // fin de compteur = fermeture du fichier
            free(time_str);
            return;
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

    // Suppression de l'utilisateur de la base de données
    char query[500];
    sprintf(query, "DELETE FROM Utilisateur WHERE ID_Utilisateur = '%s'", login);
    if (mysql_query(conn, query))
    {
        perror("Erreur de suppression dans la base de données");
        fprintf(log_file, "Erreur de suppression dans la base de données: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Utilisateur supprimé avec succès de la base de données.\n");
    }

    // Suppression de l'utilisateur du système Ubuntu
    char command[500];
    sprintf(command, "userdel -r %s", login);
    if (system(command) != 0)
    {
        fprintf(log_file, "Erreur lors de la suppression de l'utilisateur dans le système.\n");
    }
    else
    {
        printf("Utilisateur supprimé avec succès du système.\n");
    }
    fclose(log_file);
    fclose(log_file);
    free(time_str);
}

void blocage_compte(MYSQL *conn, char *username)
{

    int choix_type;     // variable pour type d'utilisateur
    char type_user[15]; // nom du type utilisateur choisi
    char login[100], raison[101];

    int user_group = get_user_type(conn, username); // groupe de l'utilisateur executant la commande

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
        strcpy(type_user, "Inscrit");
        break;
    case 2: // adminsite
        strcpy(type_user, "AdminSite");
        break;
    case 3: // admingeneral
        strcpy(type_user, "AdminGeneral");
        break;
    }

    // Ouvrez le fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "a");
    if (log_file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }

    char *time_str;
    qui_et_quand(&username, &time_str);

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
            return;
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
            return;
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
