#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include "qui_et_quand.h"
#include "gestion_entier.h"
#include "prototype_admin_general.h"
#include <mysql/mysql.h>

#define ALLOWED_CHARS_FORMAT "[0-9a-zA-Z.@]" //format authorisé

int is_valid(const char *str) {
    while (*str) {
        if (!strchr(ALLOWED_CHARS_FORMAT, *str)) {
            return 0;  // caractère non autorisé trouvé = is not valid
        }
        str++; 
    }
    return 1;  // tous les caractères sont valides = is valide
}
void ajout_compte() {
    int choix_type;  // variable pour type d'utilisateur
    char type_user[15]; //nom du type utilisateur choisi

    int user_group = get_user_type(getuid());  // groupe de l'utilisateur executant la commande

    switch(user_group) //pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
        case 0: //admingeneral
            printf("Quel type d'utilisateur souhaitez-vous ajouter? [1]adherent/[2]adminsite/[3]admingeneral: ");
            do {
                printf("Le choix doit être compris entre 1 et 3\n");
                do {
                    printf("Entrez votre choix : ");
                    scanf("%d", &choix_type);  
                } while (!gestion_int(choix_type)); //verification que c'est bien un entier et pas trop grand pour le buffer
            } while (choix_type < 1 || choix_type > 3); //verification de la valeur comprise
        break;
        case 1: //adminsite
            choix_type = 1;
            break;
        default: break;
    }
    // Demander à l'administrateur quel type d'utilisateur il souhaite ajouter

    switch(choix_type)
    {
        case 1: //inscrit
            type_user="Inscrit";
            break;
        case 2: //adminsite
            type_user="AdminSite";
            break;    
        case 3: //admingeneral
            type_user="AdminGeneral";
            break;    
    }
            char login[101], password[256], nom[51], prenom[51], email[101], estChercheur[2], info_valid[2];
    // Alerte utilisateur
    printf("\\nles caractères speciaux ne pourront pas être utilisé\\n");
    printf("En cas d'erreur de saisi, veuillez remplir le formulaire dans son integralité, une validation sera demandée en fin de saisie\\n");
    do {
        // Saisie formulaire utilisateur
        int mon_compteur_login = 5;
        do {
            mon_compteur_login--;
            printf("Entrez le login : ");
            scanf("%100s", login);
            if (mon_compteur_login == 0){return}
        } while (!is_valid(login) && ( mon_compteur_login >= 0 ) );
        int mon_compteur_mp = 5;
        do {
            mon_compteur_mp--;
            printf("Entrez le mot de passe : ");
            scanf("%255s", password);
            if (mon_compteur_mp == 0){return}
        } while (!is_valid(password) && ( mon_compteur_mp >= 0 ) );
        int mon_compteur_mail = 5;
        do {
            mon_compteur_mail--;
            printf("Entrez le mail : ");
            scanf("%100s", email);
            if (mon_compteur_mail == 0){return}
        } while (!is_valid(email) && ( mon_compteur_mail >= 0 ) );
        int mon_compteur_nom = 5;
        do {
            mon_compteur_nom--;
            printf("Entrez le nom : ");
            scanf("%50s", nom);
            if (mon_compteur_nom == 0){return}
        } while (!is_valid(nom) && ( mon_compteur_nom >= 0 ) );
        int mon_compteur_prenom = 5;
        do {
            mon_compteur_prenom--;
            printf("Entrez le prenom : ");
            scanf("%50s", prenom);
            if (mon_compteur_prenom == 0){return}
        } while (!is_valid(prenom) && ( mon_compteur_prenom >= 0 ) );
        int mon_compteur_cherch = 5;
        do {
            mon_compteur_cherch--;
            printf("Est-ce un chercheur ? (oui/non) : ");
            scanf("%1s", estChercheur);
            if (mon_compteur_cherch== 0){return}
        } while ((strcmp(estChercheur, "o") != 0 && strcmp(estChercheur, "n") != 0) && ( mon_compteur_cherch >= 0 ) );

        printf("Le login est %d, le mot de passe est %d, son groupe est %d, le mail est %d, le nom est %d, le prenom est %d, la personne est chercheur : %d",login, password, type_user, email, nom, prenom, estChercheur);
        do {
            printf ("Validez-vous ces informations? o/n");
            scanf("%1s", info_valid);
        } while (strcmp(info_valid, "o" ) != 0 && strcmp(info_valid, "n") != 0);
    } while (strcmp(info_valid, "n" ));

    // Ouverture du fichier de log
    FILE *log_file = fopen("/var/log/user_bibliotech", "a");
    if (log_file == NULL) {
        perror("Erreur lors de l'ouverture du fichier de log");
        exit(1);
    }
    fprintf(log_file, "Exécuté par: %s, Date et heure: %s\n", username, time_str);


    //  Ajout de l'utilisateur dans la base de donnée //Modifier pour mail
    char query[500];
    sprintf(query, "INSERT INTO Utilisateur (ID_Utilisateur, Nom, Prenom, MotDePasse, TypeUtilisateur, EstChercheur) VALUES ('%s','%s', '%s', '%s', '%s', '%s')",
            login, nom, prenom, password, type_user, (strcmp(estChercheur, "o") == 0) ? "1" : "0");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur d'insertion: %s\n", mysql_error(conn));
    } else {
        printf("Utilisateur ajouté avec succès à la base de données.\n");
    }
    fprintf(log_file, "Utilisateur créé dans la base de donnée: %s, Groupe: %s\n", login, type_user); //log ajout bdd
    // Création de l'utilisateur sur le système
    char username[101], user_password[256];
    strcpy(username, login);
    strcpy(user_password, password);

    // Exécutez la commande pour ajouter l'utilisateur
    char command[500];
    sprintf(command, "useradd -m -d /home/bibliotech/%s/ -s /home/bibliotech/%s/demarrage_app.sh -G %s %s", type_user, type_user, type_user, username);

    // Vérification du succès de l'exécution de la commande
    if (system(command) != 0) {
        fprintf(stderr, "Erreur lors de l'ajout de l'utilisateur sur le système.\n");
        fprintf(log_file, "Erreur lors de l'ajout de l'utilisateur sur le système.\n"); //log erreur sys
        exit(1);
    }
    fprintf(log_file, "Utilisateur créé dans le systeme: %s, Groupe: %s\n", login, type_user); //log ajout sys
    // Ajout du mot de passe utilisateur
    system(command);
    system("echo \"%s:%s\" | chpasswd", username, user_password); // Cela utilise chpasswd pour définir le mot de passe de l'utilisateur
    fprintf(log_file, "Mot de passe de l'utilisateur attribué: %s, Groupe: %s\n", login, type_user); //log mp sys

    // Fermetures
    fclose(log_file);
    free(time_str);
}