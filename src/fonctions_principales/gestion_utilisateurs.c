#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

#define ALLOWED_CHARS_FORMAT "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.@" // format authorisé

int is_valid(const char *str)
{
    while (*str)
    {
        if (!strchr(ALLOWED_CHARS_FORMAT, *str))
        {
            printf("caractère non autorisé\n");
            return 0; // caractère non autorisé trouvé = is not valid
        }
        str++;
    }
    return 1; // tous les caractères sont valides = is valide
}

void ajout_compte(MYSQL *conn, char *username)
{
    system("clear");
    int choix_type;     // variable pour type d'utilisateur
    char type_user[15]; // nom du type utilisateur choisi
    char site[10]; //Site pour adminsite
    int user_group = get_user_group(conn);

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        do {
            printf("Quel type d'utilisateur souhaitez-vous ajouter? \n");
            printf("-------------------------------------------\n");
            printf("[1]adherent\n[2]adminsite\n[3]admingeneral \n");

            do {
                printf("Entrez votre choix : ");
                scanf("%d", &choix_type);
                if (!gestion_int(choix_type)) {
                    printf("Veuillez entrer un entier valide.\n");
                }
            } while (choix_type < 1 || choix_type > 3);

            if (choix_type < 1 || choix_type > 3) {
                printf("Le choix doit être compris entre 1 et 3.\n");
            }
        } while (choix_type < 1 || choix_type > 3);
        break;
    case 2: // adminsite
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
        printf("\nQUel est le site de l'utilisateur?\nSite [A / B / C]?");
        scanf("%s", site);
        if (strcmp(site, "A") == 0) { strcpy(site, "Site A"); };
        if (strcmp(site, "B") == 0) { strcpy(site, "Site B"); };
        if (strcmp(site, "C") == 0) { strcpy(site, "Site C"); };
        break;
    case 3: // admingeneral
        strcpy(type_user, "AdminGeneral");
        break;
    }
    char login[101], password[256], nom[51], prenom[51], estChercheur[2], info_valid[2];
    // Alerte utilisateur
    printf("\nATTENTION : Les caractères spéciaux ne pourront pas être utilisés.\n");
    printf("\n Veuillez remplir le formulaire dans son integralité, une validation sera demandée en fin de saisie.\n");
    do
    {
        // Saisie formulaire utilisateur
        int mon_compteur_login = 5;
        do
        {
            mon_compteur_login--;
            printf("Entrez l'adresse (qui fera office de login) : ");
            scanf("%100s", login);
            //printf("login debut : %s\n", login);
            if (mon_compteur_login == 0)
            {
                return;
            }
        } while ((is_valid(login)==0) && (mon_compteur_login >= 0));

        printf("login : %s\n", login);

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
            printf("Est-ce un chercheur ? (o/n) : ");
            scanf("%1s", estChercheur);
            if (mon_compteur_cherch == 0)
            {
                return;
            }
        } while ((strcmp(estChercheur, "o") != 0 && strcmp(estChercheur, "n") != 0) && (mon_compteur_cherch >= 0));

        printf("\nVérification des informations :\n");
        printf("---------------------------------\n");
        printf("Login          : %s\n", login);
        printf("Mot de passe   : %s\n", password);
        printf("Groupe         : %s\n", type_user);
        printf("Nom            : %s\n", nom);
        printf("Prénom         : %s\n", prenom);
        printf("Est chercheur  : %s\n", (strcmp(estChercheur, "o") == 0) ? "Oui" : "Non");
        printf("---------------------------------\n");
        do
        {
            printf("--> Validez-vous ces informations? o/n : ");
            scanf("%1s", info_valid);
            if (strcmp(info_valid, "n") == 0)
            {
                break;
            }
        } while (strcmp(info_valid, "o") != 0);

    } while (strcmp(info_valid, "n") == 0);

    // Ouverture du fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "r");
    if (log_file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }

    time_t now;
    time(&now);
    fprintf(log_file, "Exécuté par: %s, Date et heure: %s\n", username, ctime(&now));

    //copie des infos pour le systeme
    char user_password[256];
    strcpy(username, login);
    strcpy(user_password, password);

    //  Ajout de l'utilisateur dans la base de donnée //Modifier pour mail
    char query[1024];
    if ((strcmp(site, "Site A") == 0) || (strcmp(site, "Site B") == 0)||(strcmp(site, "Site C") == 0) ){
    sprintf(query, "INSERT INTO Utilisateur (Email, Nom, Prenom, MotDePasse, TypeUtilisateur, EstChercheur, SitePrincipal) VALUES ('%s','%s', '%s', '%s', '%s', '%s', '%s')",
            login, nom, prenom, password, type_user, (strcmp(estChercheur, "o") == 0) ? "1" : "0", site);
    }else{
    sprintf(query, "INSERT INTO Utilisateur (Email, Nom, Prenom, MotDePasse, TypeUtilisateur, EstChercheur) VALUES ('%s','%s', '%s', '%s', '%s', '%s')",
            login, nom, prenom, password, type_user, (strcmp(estChercheur, "o") == 0) ? "1" : "0");
    }
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Erreur d'insertion: %s\n", mysql_error(conn));
        sleep(5);
    }
    else
    {
        system("clear");   
        printf("Utilisateur ajouté à la bibliothèque.\n");
        sleep(5);
    }
    fprintf(log_file, "Utilisateur créé dans la base de données bibliotèque : %s, Groupe: %s\n", login, type_user); // log ajout bdd

    // Ajout de l'utilisateur dans le serveur   
    char createMaria[1024];
    sprintf(createMaria, "CREATE USER '%s'@'localhost' IDENTIFIED BY ''", login);
    if (mysql_query(conn, createMaria))
    {
        fprintf(stderr, "Erreur lors de la création de l'utilisateur : %s\n", mysql_error(conn));
        sleep(5);
    }
    else
    {
        printf("Utilisateur créé dans Mariadb avec succès .\n");

        char grantMaria[1024];
        sprintf(grantMaria, "GRANT ALL PRIVILEGES ON *.* TO '%s'@'localhost' WITH GRANT OPTION", login);
        if (mysql_query(conn, grantMaria))
        {
            fprintf(stderr, "Erreur lors de l'accord des privilèges : %s\n", mysql_error(conn));
            sleep(5);
        }
        else
        {
            printf("Privilèges accordés avec succès.\n");
            sleep(5);
        }
    }



    // Création de l'utilisateur sur le système
    // Exécutez la commande pour ajouter l'utilisateur
    char add_user_command[500];
    sprintf(add_user_command, "/usr/local/bin/bibli_multisite/script/add_user.sh %s %s", type_user, username);
    if (system(add_user_command) != 0)
    {
        fprintf(stderr, "Erreur lors de l'exécution du script d'ajout d'utilisateur);
        fclose(log_file);
        sleep(5);
        exit(1);
    }
    else
    {
        fprintf(log_file, "Utilisateur créé dans le systeme : %s, Groupe: %s\n", login, type_user); // log ajout sys
        //char passwd[500];
        char chpasswd_command[500];
        sprintf(chpasswd_command, "echo '%s:%s' | sudo chpasswd", username, user_password);
        system(chpasswd_command);
        //system(passwd);                                                        // Cela utilise chpasswd pour définir le mot de passe de l'utilisateur
        fprintf(log_file, "Mot de passe de l'utilisateur attribué : %s, Groupe: %s\n", login, type_user); // log mp sys
        system("clear");
        printf("\nUtilisateur créé \n");
        // Fermetures
    }

    fclose(log_file);
}

void suppression_compte(MYSQL *conn, char *username)
{

    system("clear");

    int choix_type; // variable pour type d'utilisateur
    char login[101];
    char type_user[15]; // nom du type d'utilisateur

    int user_group = get_user_group(conn); // groupe de l'utilisateur executant la commande

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        printf("Quel type d'utilisateur souhaitez-vous supprimer? \n");
        printf("-------------------------------------------\n");
        printf("[1]adherent\n[2]adminsite\n[3]admingeneral \n");
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
    case 2: // adminsite
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
    fprintf(log_file, "Tentative de suppression par l'utilisateur : %s, Date et heure: %s\n", username, time_str);
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
    } while ((is_valid(login)==0) && (mon_compteur_log >= 0));

    // Vérifiez si l'utilisateur existe
    char query_select[500];
    sprintf(query_select, "SELECT * FROM Utilisateur WHERE Email = '%s'", login);
    if (mysql_query(conn, query_select))
    {
        fprintf(log_file, "Erreur lors de la vérification de l'utilisateur dans la base de données : %s\n", mysql_error(conn));
        printf("Erreur d'identification de l'utilisateur dans la base de données.\n");
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

    //copie du nom pour la variable system
    char system_login[101];
    strcpy(system_login, login);

    // Suppression de l'utilisateur de la base de données
    char query[500];
    sprintf(query, "DELETE FROM Utilisateur WHERE Email = '%s'", login);
    if (mysql_query(conn, query))
    {
        perror("Erreur de suppression dans la base de données\n");
        fprintf(log_file, "Erreur de suppression dans la base de données: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Utilisateur supprimé avec succès de la base de données.\n");
    }

    // Suppression de l'utilisateur du système Ubuntu
    char remove_user_command[500];
    sprintf(remove_user_command, "/usr/local/bin/bibli_multisite/script/remove_user.sh %s", system_login);
    if (system(remove_user_command) != 0)
    {
        fprintf(stderr, "Erreur lors de l'exécution du script de suppression d'utilisateur.\n");
    }
    printf("Utilisateur supprimé du système\n");
    fclose(log_file);
    free(time_str);
    sleep(5);
}

void blocage_compte(MYSQL *conn, char *username)
{
    system("clear");

    int choix_type;     // variable pour type d'utilisateur
    char type_user[15]; // nom du type utilisateur choisi
    char login[100], raison[101];

    int user_group = get_user_group(conn); // groupe de l'utilisateur executant la commande

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        printf("Quel type d'utilisateur souhaitez-vous bloquer? \n");
        printf("-------------------------------------------\n");
        printf("[1]adherent\n[2]adminsite\n[3]admingeneral \n");
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
    case 2: // adminsite
        choix_type = 1;
        break;
    default:
        return;
        break;
    }
    // l'administrateur a choisi quel type d'utilisateur il souhaite bloquer

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
    fprintf(log_file, "Tentative de blocage par l'utilisateur : %s, Date et heure: %s\n", username, time_str);
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
    } while ((is_valid(login)==0) && (mon_compteur_log >= 0));
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
    } while ((is_valid(login)==0) && (mon_compteur_log >= 0));

    // Vérifiez si l'utilisateur existe
    char query_select[500];
    sprintf(query_select, "SELECT * FROM Utilisateur WHERE ID_Utilisateur = '%s'", login);
    if (mysql_query(conn, query_select))
    {
        fprintf(log_file, "Erreur lors de la vérification de l'utilisateur dans la base de données : %s\n", mysql_error(conn));
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
        fprintf(log_file, "Erreur de blocage dans la base de données : %s\n", mysql_error(conn));
    }
    else
    {
        printf("Utilisateur bloqué avec succès de la base de données.\n");
    }

    // Suppression de l'utilisateur du système Ubuntu
    char lock_user_command[500];
    sprintf(lock_user_command, "/usr/local/bin/bibli_multisite/script/lock_user.sh %s", login);
    if (system(lock_user_command) != 0)
    {
        fprintf(stderr, "Erreur lors de l'exécution du script de blocage d'utilisateur.\n");
    }
    else
    {
        system("clear");
        printf("\nUtilisateur bloqué avec succès dans le système.\n");
        fprintf(log_file, "Utilisateur %s bloqué avec succès\n", login);
    }

    fclose(log_file);
    free(time_str);
}
