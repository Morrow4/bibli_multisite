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

    //Fonction Compter le nombre de livre par titre
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
//Je verifie que le compte est bien un adherent
void Emprunt_soimeme() {
    int numLivre; //variable de numerotation de livre affiché
    int tailleTab; //variable pour le nombre de livre correspondant à un titre
    int choix_type;  // variable pour type d'utilisateur
    char type_user[15]; //nom du type utilisateur choisi
    int user_group = get_user_type(getuid());  // groupe de l'utilisateur executant la commande 2=adherent
    if (user_group != 2){
        return; //besoin de logguer ici
    }else{
    printf("Avez-vous son ISBN? (o/n) Appuyez sur tout autre touche pour sortir du menu : ");
    char reponse;
    scanf(" %c", &reponse);
    //si "n" passer sur un formulaire de saisi de titre
    if (reponse == 'n' || reponse == 'N') {
        printf("Saisissez le titre du livre : ");
        scanf(" %[^\n]", titreSaisi);
        //Appel de la fonction compter le nombre de livre par titre
        nombreLivresParTitre(titreSaisi);
        //Affichage dans un tableau le numero d'affichage des livres, titre, editions, isbn
        printf("Numéro  Titre                          Edition                       ISBN\n");
        printf("---------------------------------------------------------------------------\n");

        for (int i = 0; i < nombreLivres; i++) {
            printf("%-7d%-30s%-30s%-15s\n", i + 1, livres[i].Titre, livres[i].Edition, livres[i].ISBN);
        }
        // Récupérer les livres
        while ((row = mysql_fetch_row(res)) != NULL) {
            sscanf(row[0], "%s", livres[nombreLivres].ISBN);
            sscanf(row[1], "%s", livres[nombreLivres].Titre);
            sscanf(row[2], "%s", livres[nombreLivres].Auteur);
            sscanf(row[3], "%s", livres[nombreLivres].Edition);
            sscanf(row[4], "%s", livres[nombreLivres].Genre);
            nombreLivres++;
        }
        //Saisi du numero du livre
        printf("Quel est le numero du livre concerné par la demande? (saisissez un chiffre)\n");
        scanf("%s", numLivre)
        //Verification que le numero saisi correspond au nombre de livre affiché
        if(0<numLivre<tailleTab){
        //recupération de l'ISBN en variable "ISBN_true"
        }else{
            return 0;
        }
    } else if(reponse == 'o' || reponse == 'O') {
        //Comparaison de ISBN_test avec ceux de la base de donnée
        //si ISBN ne correspond pas, afficher "Aucun livre trouver"
        //Afficher : Veuillez saisir à nouveau l'ISBN
        //si ISBN correspond alors ISBN_true=ISBN_test
    } else {
        // afficher erreur de saisi, Sorti du menu
    }
    //A partir de la valeur ISBN_true
    //Verification du nombre d'exemplaire possible
    //Si le nombre d'exemplaire est égal à 0,
    //Verifier la date de reservation du 
    //impossible de réserver le livre avant tel date, retour au menu
    //Si le nombre d'exemplaire est superieur à 0 alors : 
    //D'abord on réduit le nombre d'exemplaire présent dans la base de donnée
    //Et on ajoute dans la table emprunt les informations necessaire : 
    //qui ressemble à : 
    //CREATE TABLE Emprunt (
    //ID_Emprunt INT PRIMARY KEY AUTO_INCREMENT,
    //ID_Exemplaire INT,
    //ID_Utilisateur VARCHAR(100),
    //SiteDeRestitution VARCHAR(50),
    //DateEmprunt TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    //CONSTRAINT fk_Emprunt_Livre FOREIGN KEY (ID_Exemplaire) REFERENCES Exemplaire(ID_Exemplaire));
    //la table exemplaire ressemble à : 
    //CREATE TABLE Exemplaire (
    //    ID_Exemplaire INT PRIMARY KEY AUTO_INCREMENT,
    //    ISBN VARCHAR(13),
    //    Disponibilite BOOLEAN DEFAULT true,
    //    SitePrincipal VARCHAR(50),
    //    EstLivrePourChercheur BOOLEAN DEFAULT false,
    //    CONSTRAINT chk_Disponibilite CHECK (Disponibilite IN (true, false)),
    //    CONSTRAINT fk_Exemplaire_Livre FOREIGN KEY (ISBN) REFERENCES Livre(ISBN) ON DELETE CASCADE ON UPDATE CASCADE
    //);
    //Donc à partir de l'ISBN il faudra indiquer : 
    //Le site de restitution, la date d'emprunt qui correspond à celle du jour
    //je ne sait pas comment se gere les auto incremente
    //l'ID_Utilisateur correspond à l'utilisateur actuel de l'application

}
