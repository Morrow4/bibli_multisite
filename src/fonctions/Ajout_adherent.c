#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void Ajout_adherent() {
    char login[101], password[256], nom[51], prenom[51], email[101], estChercheur[2];
    // Alerte utilisateur
    printf("\\nles caractères speciaux ne pourront pas être utilisé\\n");
    printf("En cas d'erreur de saisi, veuillez remplir le formulaire dans son integralité, une validation sera demandée en fin de saisie\\n");
    // Saisie des informations de l'utilisateur
    do {
        printf("Entrez le login : ");
        scanf("%100s", login);
    } while (!is_valid(login));

    do {
        printf("Entrez le mot de passe : ");
        scanf("%255s", password);
    } while (!is_valid(password));

    do {
        printf("Entrez le mail : ");
        scanf("%100s", email);
    } while (!is_valid(email));

    do {
        printf("Entrez le nom : ");
        scanf("%50s", nom);
    } while (!is_valid(nom));

    do {
        printf("Entrez le prenom : ");
        scanf("%50s", prenom);
    } while (!is_valid(prenom));

    do {
        printf("Est-ce un chercheur ? (oui/non) : ");
        scanf("%1s", estChercheur);
    } while (strcmp(estChercheur, "o") != 0 && strcmp(estChercheur, "n") != 0);

}

    // Insertion des informations de l'utilisateur dans la table Utilisateur
    char query[500];
    sprintf(query, "INSERT INTO Utilisateur (ID_Utilisateur, Nom, Prenom, Email, MotDePasse, TypeUtilisateur, EstChercheur) VALUES ('%s','%s', '%s', '%s', '%s', 'Inscrit', %s)",
            login, nom, prenom, email, password, (strcmp(estChercheur, "o") == 0) ? "1" : "0");

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Erreur d'insertion: %s\n", mysql_error(conn));
    } else {
        printf("Utilisateur ajouté avec succès à la base de données.\n");
    }

    // Création de l'utilisateur sur le système

    char username[101], user_password[256];
    strcpy(username, login);
    strcpy(user_password, password);

    // Exécutez la commande pour ajouter l'utilisateur
    char command[500];
    sprintf(command, "useradd -m -d /home/bibliotech/Invite/ -s /home/bibliotech/Invite/demarrage_app.sh -G inscrit %s", username);

    // Vérification du succès de l'exécution de la commande
    if (system(command) != 0) {
        fprintf(stderr, "Erreur lors de l'ajout de l'utilisateur sur le système.\n");
        exit(1);
    }
    
    // Ajout du mot de passe utilisateur
    system(command);
    system("echo \"%s:%s\" | chpasswd", username, user_password); // Cela utilise chpasswd pour définir le mot de passe de l'utilisateur


    // Fermeture de la connexion à la base de données
    mysql_close(conn);
}

int main() {
    Ajout_adherent();
    return 0;
}
