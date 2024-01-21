#ifndef FONCTIONS_BDD_H
#define FONCTIONS_BDD_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure pour stocker les informations d'un livre
typedef struct Livre
{
    char ISBN[14];
    char Titre[255];
    char Auteur[100];
    char Edition[100];
    char Genre[100];
} Livre;

//------Les fonctions à faire-----------
//---------------------------------------
//Liste_livres_et_dispo.c
void Liste_livres_et_dispo(MYSQL *conn);
//Inscri_liste_attente_livre.c
void Inscri_liste_attente_livre();
//--------------------------------

// gestion_livres.c
void ajout_livre(MYSQL *conn);
void mise_a_jour_livre(MYSQL *conn, char *ISBN);
void suppression_livre(MYSQL *conn, char *ISBN);
void recherche_ISBN(MYSQL *conn, char *titre, char *auteur);

// reservation.c
bool au_moins_un_exemplaire_disponible(MYSQL *conn, char *isbn_livre);
int obtenir_id_exemplaire_disponible(MYSQL *conn, char *isbn_livre);
void mettre_a_jour_disponibilite_exemplaire(MYSQL *conn, int id_exemplaire, bool disponibilite);
void enregistrer_reservation(MYSQL *conn, char *email_utilisateur, int id_exemplaire);

// restitution.c
void verifier_et_valider_restitution(MYSQL *conn, int id_restitution);
int valider_restitution(MYSQL *conn, int id_emprunt, const char *site_restitution);
void marquer_restitution_en_transit(MYSQL *conn, int id_emprunt, const char *site_restitution);
void marquer_exemplaire_disponible(MYSQL *conn, int id_emprunt);
void mettre_a_jour_date_restitution(MYSQL *conn, int id_restitution);

// gestion_utilisateurs.c
int is_valid(const char *str);
void Suppression_compte();
void ajout_compte();
void Blocage_compte();

// emprunt_reservation.c
void reserver_livre(MYSQL *conn, char *email_utilisateur, char *isbn_livre);
void emprunter_livre(MYSQL *conn, char *email_utilisateur, int id_reservation);
bool reservation_existe_et_valide(MYSQL *conn, int id_reservation);
int obtenir_id_exemplaire_reservé(MYSQL *conn, int id_reservation);
void mettre_a_jour_est_reserve_reservation(MYSQL *conn, int id_reservation, bool est_reserve);
void enregistrer_emprunt(MYSQL *conn, char *email_utilisateur, int id_exemplaire);

// Emprunt_soimeme.c
int nombreLivresParTitre(const char *titreRecherche);
void afficherDetailsLivre(const Livre *livre);
void effectuerEmprunt(MYSQL *conn, const char *ISBN, const char *username);
void verifierEtEffectuerEmprunt(MYSQL *conn, const char *ISBN, const char *username);
void Emprunt_soimeme(MYSQL *conn);

// Exemplaires.c
void ajout_exemplaire(MYSQL *conn);
void mise_a_jour_exemplaire(MYSQL *conn, int id_exemplaire);
void suppression_exemplaire(MYSQL *conn, int id_exemplaire);
void recherche_exemplaire(MYSQL *conn, char *ISBN);

#endif