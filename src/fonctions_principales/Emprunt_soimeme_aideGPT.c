#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prototype_inscrit.h"
#include <mysql/mysql.h>

// Structure pour stocker les informations d'un livre
typedef struct {
    char ISBN[14];
    char Titre[255];
    char Auteur[100];
    char Edition[100];
    char Genre[100];
} Livre;

// Déclaration de variables globales
MYSQL_RES *result;
MYSQL_ROW row;

// Fonction Compter le nombre de livres par titre
int nombreLivresParTitre(const char *titreRecherche) {
    int nombreLivres = 0;
    char query[255];
    sprintf(query, "SELECT COUNT(*) FROM Livre WHERE Titre = '%s'", titreRecherche);
    result = mysql_store_result(conn);
    if ((row = mysql_fetch_row(result)) != NULL) {
        sscanf(row[0], "%d", &nombreLivres);
    }
    return nombreLivres;
}

// Fonction pour afficher les détails du livre
void afficherDetailsLivre(const Livre *livre) {
    printf("%-7d%-30s%-30s%-15s\n", i + 1, livre->Titre, livre->Edition, livre->ISBN);
}

// Fonction pour vérifier et effectuer l'emprunt
void verifierEtEffectuerEmprunt(MYSQL *conn, const char *ISBN) {
    double joursDeRetard;
    int livreTrouve = trouverRetard(conn, ISBN, &joursDeRetard);

    if (livreTrouve) {
        if (joursDeRetard > 14) {
            printf("Livre trouvé mais en retard de %.2f jours.\n", joursDeRetard);
            double joursRestants = 14 - joursDeRetard;
            printf("Il reste %.2f jours avant la date limite.\n", joursRestants);
        } else {
            printf("Livre trouvé, pas de retard.\n");
        }

        // Continuer le processus d'emprunt
        effectuerEmprunt(conn, ISBN);
    } else {
        printf("Livre non trouvé, veuillez contacter un administrateur.\n");
    }
}

// Fonction pour l'emprunt de livre
void Emprunt_soimeme(MYSQL *conn) {
    int numLivre;
    int tailleTab;
    int user_group = get_user_type(getuid());

    if (user_group != 2) {
        return;  // Besoin de logguer ici
    }

    printf("Avez-vous son ISBN? (o/n) Appuyez sur tout autre touche pour sortir du menu : ");
    char reponse;
    scanf(" %c", &reponse);

    if (reponse == 'n' || reponse == 'N') {
        printf("Saisissez le titre du livre : ");
        char titreSaisi[255];
        scanf(" %[^\n]", titreSaisi);

        // Appel de la fonction compter le nombre de livres par titre
        tailleTab = nombreLivresParTitre(conn, titreSaisi);

        // Affichage dans un tableau le numéro d'affichage des livres, titre, éditions, isbn
        printf("Numéro  Titre                          Edition                       ISBN\n");
        printf("---------------------------------------------------------------------------\n");

        // Récupérer les livres
        for (int i = 0; i < tailleTab; i++) {
            afficherDetailsLivre(&livres[i]);
        }

        // Saisie du numéro du livre
        printf("Quel est le numéro du livre concerné par la demande? (saisissez un chiffre)\n");
        scanf("%d", &numLivre);

        // Vérification que le numéro saisi correspond au nombre de livres affiché
        if (0 < numLivre && numLivre <= tailleTab) {
            // Récupération de l'ISBN en variable "ISBN_true"
            char ISBN_true[14];
            strcpy(ISBN_true, livres[numLivre - 1].ISBN);

            // Vérifier et effectuer l'emprunt
            verifierEtEffectuerEmprunt(conn, ISBN_true);
        } else {
            printf("Numéro de livre incorrect. Sortie du menu.\n");
        }
    } else if (reponse == 'o' || reponse == 'O') {
        char ISBN_test[14];
        // Comparaison de ISBN_test avec ceux de la base de données
        printf("Veuillez saisir l'ISBN du livre : ");
        scanf(" %s", ISBN_test);

        // Vérifier et effectuer l'emprunt
        verifierEtEffectuerEmprunt(conn, ISBN_test);
    } else {
        // Sortir du menu
        return;
    }
}
