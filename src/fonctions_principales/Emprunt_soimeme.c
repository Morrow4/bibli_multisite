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

// Fonction pour effectuer l'emprunt
void effectuerEmprunt(MYSQL *conn, const char *ISBN) {

    char query[255];
    // Réduire le nombre d'exemplaires disponibles
    sprintf(query, "UPDATE Exemplaire SET Disponibilite = false WHERE ISBN = '%s' AND Disponibilite = true LIMIT 1", ISBN);
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Erreur lors de la mise à jour du nombre d'exemplaires\n");
        return;
    }

    // Exemple : Insérer une nouvelle entrée dans la table Emprunt
    //récupération de l'ID_Utilisateur = Email
    //récupération de l'ID_Exemplaire
    //recupération du SitedeRestitution
    //DateEmprunt = Date du Jour
    sprintf(query, "INSERT INTO Emprunt (ID_Exemplaire, ID_Utilisateur, SiteDeRestitution, DateEmprunt) VALUES ((SELECT ID_Exemplaire FROM Exemplaire WHERE ISBN = '%s' AND Disponibilite = false LIMIT 1), 'ID_Utilisateur_Actuel', 'SiteDeRestitution_À_Définir', NOW())", ISBN);
    if (mysql_query(conn, query) != 0) {
        fprintf(stderr, "Erreur lors de l'ajout de l'emprunt\n");
        return;
    }
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
        char *pointeurTitresaisi = titresaisi;
        scanf(" %[^\n]", titreSaisi);

        // Appel de la fonction compter le nombre de livres par titre
        tailleTab = nombreLivresParTitre(pointeurTitresaisi);

        // Affichage dans un tableau le numéro d'affichage des livres, titre, éditions, isbn
        printf("Numéro  Titre                          Edition                       ISBN\n");
        printf("---------------------------------------------------------------------------\n");

        // Récupérer les livres
        for (int i = 0; i < tailleTab; i++) {
            afficherDetailsLivre(&Livre[i]);
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
