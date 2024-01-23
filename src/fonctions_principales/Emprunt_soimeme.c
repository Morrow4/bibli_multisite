#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <pwd.h>
#include "../header/fonctions_bdd.h"
#include "../header/utilitaire.h"

// Déclaration de variables globales
MYSQL_RES *result;
MYSQL_ROW row;
MYSQL *conn;

// Fonction Compter le nombre de livres par titre
int nombreLivresParTitre(const char *titreRecherche)
{
    printf("je rentre dans la fonction\n");
    int nombreLivres = 0;
    MYSQL_RES *result;
    MYSQL_ROW row;

    char query[255];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM Livre WHERE Titre = '%s'", titreRecherche);

    // Exécutez la requête SQL
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur lors de l'exécution de la requête SQL : %s\n", mysql_error(conn));
        return nombreLivres;
    }

    // Récupérez le résultat de la requête
    result = mysql_store_result(conn);

    // Vérifiez si le résultat est non nul
    if (result != NULL)
    {
        // Récupérez la première ligne du résultat
        if ((row = mysql_fetch_row(result)) != NULL)
        {
            sscanf(row[0], "%d", &nombreLivres);
        }

        // Libérez le résultat après l'avoir utilisé
        mysql_free_result(result);
    }
    else
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
    }

    return nombreLivres;
}

// Fonction pour afficher les détails du livre
void afficherDetailsLivre(const Livre *livre)
{
    printf("%-30s%-30s%-15s\n", livre->Titre, livre->Edition, livre->ISBN);
}

// Fonction pour effectuer l'emprunt
void effectuerEmprunt(MYSQL *conn, const char *ISBN, const char *username)
{
    printf("effectuer emprunt debut\n");
    qui(username);

    char query[255];
    // Réduire le nombre d'exemplaires disponibles
    // Verifier s'il existe des ISBN pour l'exemplaire demandé
    printf("ISBN = '%s'\n", ISBN);
    sprintf(query, "SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN = '%s' AND Disponibilite = true LIMIT 1", ISBN);
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur de verification\n");
        return;
    }
    // Stocker dans une variable
    int Var_IdExemplaire;
    result = mysql_store_result(conn);
    if ((row = mysql_fetch_row(result)) != NULL)
    {
        sscanf(row[0], "%d", &Var_IdExemplaire);
    }

    mysql_free_result(result); 
    // Maj table exemplaire
    sprintf(query, "UPDATE Exemplaire SET Disponibilite = false WHERE ID_Exemplaire = '%d' AND Disponibilite = true", Var_IdExemplaire);
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur lors de la mise à jour du nombre d'exemplaires : %s\n", mysql_error(conn));
        return;
    }

    // récupération de l'ID_Utilisateur = Email
    qui(username);
    printf("recuperation de l'id user : %s\n", username);
    printf("recuperation de l'id exemplaire : %d\n", Var_IdExemplaire);   
    sprintf(query, "INSERT INTO Emprunt (ID_Exemplaire, ID_Utilisateur) VALUES ('%d', '%s')", Var_IdExemplaire, username);
    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur lors de l'ajout de l'emprunt : %s\n", mysql_error(conn));
        return;
    }
    printf("Exemplaire emprunté!\n");
    free((char *)username);
}

//Fonction pour trouver le nombre de jour de retard
// Fonction pour trouver le retard pour un ISBN donné
/*
int trouverRetard(MYSQL *conn, const char *ISBN, double *joursDeRetard)
{
    // Requête pour récupérer la date la plus ancienne d'emprunt
    char query[255];
    sprintf(query, "SELECT MIN(DateEmprunt) FROM Emprunt WHERE ID_Exemplaire IN (SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN = '%s')", ISBN);

    if (mysql_query(conn, query) != 0)
    {
        fprintf(stderr, "Erreur lors de la récupération de la date la plus ancienne : %s\n", mysql_error(conn));
        return 0;  // Retourne 0 en cas d'erreur
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result)
    {
        fprintf(stderr, "Erreur lors de la récupération du résultat : %s\n", mysql_error(conn));
        return 0;  // Retourne 0 en cas d'erreur
    }

    MYSQL_ROW row;
    if ((row = mysql_fetch_row(result)) != NULL)
    {
        // Convertir la date récupérée en temps Unix
        time_t dateEmprunt = strtotime(row[0]);
        time_t dateDuJour = time(NULL);

        // Calculer la différence en jours
        double differenceEnSecondes = difftime(dateDuJour, dateEmprunt);
        *joursDeRetard = differenceEnSecondes / (24 * 3600);

        // Libérer le résultat après utilisation
        mysql_free_result(result);

        return 1;  // Retourne 1 si tout est OK
    }
    else
    {
        fprintf(stderr, "Aucun résultat retourné par la requête\n");
        return 0;  // Retourne 0 si aucun résultat trouvé
    }
}
*/
// Fonction pour vérifier et effectuer l'emprunt
void verifierEtEffectuerEmprunt(MYSQL *conn, const char *ISBN, const char *username)
{
    printf("verifier et effectuer emprunt : je rentre dans la fonction\n");
    double joursDeRetard;
    int livreTrouve = trouverRetard(conn, ISBN, &joursDeRetard);

    if (livreTrouve == 0)
    {
        printf("Livre non emprunté auparavant.\n");
        // Continuer le processus d'emprunt
        effectuerEmprunt(conn, ISBN, username);
        return;
    }

    if (joursDeRetard > 14)
    {
        double joursRestants = 14 - joursDeRetard;
        printf("Le livre a %.2f jours de retard.\n", joursRestants);
    }
    else
    {
        printf("Livre trouvé, pas de retard.\n");
    }

    // Continuer le processus d'emprunt
    effectuerEmprunt(conn, ISBN, username);
}


// Fonction pour l'emprunt de livre
void Emprunt_soimeme(MYSQL *conn, char *username)
{
    int numLivre;
    int tailleTab;
    sprintf(username, "%d", getuid());
    int user_group = get_user_group(conn);

    switch (user_group) // pour faire un switch il faut apparemment une variable de type int, donc j'ai modifié tout ça en conséquent
    {
    case 1: // admingeneral
        printf("Vous êtes administrateurs général, merci d'utiliser votre compte adhérent. \n");
        break;
    case 2: // adminsite
        printf("Vous êtes administrateurs site, merci d'utiliser votre compte adhérent. \n");
        return;
        break;
    case 3: //adherent
        printf("Bienvenue dans le menu d'emprunt des livres! \n");
    default:
        return;
        break;
    }

    printf("Avez-vous son ISBN? (o/n) Appuyez sur tout autre touche pour sortir du menu : ");
    char reponse;
    scanf(" %c", &reponse);

    if (reponse == 'n' || reponse == 'N')
    {
        printf("Saisissez le titre du livre : ");
        char titreSaisi[255];
        scanf(" %[^\n]", titreSaisi);

        // Appel de la fonction compter le nombre de livres par titre
        tailleTab = nombreLivresParTitre(titreSaisi);

        // Affichage dans un tableau le numéro d'affichage des livres, titre, éditions, isbn
        printf("Numéro  Titre                          Edition                       ISBN\n");
        printf("---------------------------------------------------------------------------\n");

        // Récupérer les livres
        Livre *Livres = malloc(tailleTab * sizeof(Livre));
        if (Livres == NULL)
        {
            fprintf(stderr, "Erreur d'allocation mémoire\n");
            printf("L'erreur à la récupération de livre");
            free(Livres);
            return;
        }

        for (int i = 0; i < tailleTab; i++)
        {
            afficherDetailsLivre(&Livres[i]);
        }

        // Saisie du numéro du livre
        printf("Quel est le numéro du livre concerné par la demande? (saisissez un chiffre)\n");
        scanf("%d", &numLivre);

        // Vérification que le numéro saisi correspond au nombre de livres affiché
        if (0 < numLivre && numLivre <= tailleTab)
        {
            // Récupération de l'ISBN en variable "ISBN_true"
            char ISBN_true[14];
            strcpy(ISBN_true, Livres[numLivre - 1].ISBN);

            // Vérifier et effectuer l'emprunt
            verifierEtEffectuerEmprunt(conn, ISBN_true, username);
        }
        else
        {
            printf("Numéro de livre incorrect. Sortie du menu.\n");
        }
    }
    else if (reponse == 'o' || reponse == 'O')
    {
        char ISBN_test[15];
        // Comparaison de ISBN_test avec ceux de la base de données
        printf("Veuillez saisir l'ISBN du livre : ");
        scanf(" %s", ISBN_test);

        // Vérifier et effectuer l'emprunt
        struct passwd *pwd = getpwuid(getuid());
        char *username = pwd->pw_name;
        verifierEtEffectuerEmprunt(conn, ISBN_test, username);
    }
    else
    {
        return;
    }
}
