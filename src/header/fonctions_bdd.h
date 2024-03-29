#ifndef FONCTIONS_BDD_H
#define FONCTIONS_BDD_H

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// gestion_livres.c
void ajout_livre(MYSQL *conn);
void mise_a_jour_livre(MYSQL *conn);
void suppression_livre(MYSQL *conn);
void afficher_tous_les_livres(MYSQL *conn);
void escapeString(MYSQL *conn, const char *source, char *destination, size_t dest_size);
void rechercherLivreParTitre(MYSQL *conn);
void rechercherLivreParAuteur(MYSQL *conn);

// gestion_reservation.c
void reserver_livre(MYSQL *conn, char *email_utilisateur);
bool au_moins_un_exemplaire_disponible(MYSQL *conn, char *isbn_livre);
int obtenir_id_exemplaire_disponible(MYSQL *conn, char *isbn_livre);
void mettre_a_jour_disponibilite_exemplaire(MYSQL *conn, int id_exemplaire, bool disponibilite);
void enregistrer_reservation(MYSQL *conn, char *email_utilisateur, int id_exemplaire);
void afficher_reservations_utilisateur(MYSQL *conn, char *email_utilisateur);
void annuler_reservation_par_id(MYSQL *conn, char *email_utilisateur);
bool reservation_existe_et_valide(MYSQL *conn, int id_reservation);
int obtenir_id_exemplaire_de_reservation(MYSQL *conn, int id_reservation);
void emprunter_livre_apres_reservation(MYSQL *conn, char *email_utilisateur);
void mettre_a_jour_est_reserve_reservation(MYSQL *conn, int id_reservation, bool est_reserve);
void enregistrer_emprunt(MYSQL *conn, char *email_utilisateur, int id_exemplaire);

// gestion_restitution.c
void enregistrer_restitution(MYSQL *conn, char *email_utilisateur);
void verifier_et_valider_restitution(MYSQL *conn, int id_restitution);
int valider_restitution(MYSQL *conn, int id_emprunt, char *site_restitution);
void marquer_restitution_en_transit(MYSQL *conn, int id_emprunt, char *site_restitution);
void marquer_exemplaire_disponible(MYSQL *conn, int id_emprunt);
void mettre_a_jour_date_restitution(MYSQL *conn, int id_restitution);
void afficher_livres_restitues_apres_transit(MYSQL *conn);
void valider_restitution_apres_transit(MYSQL *conn);
int obtenir_id_emprunt_de_restitution(MYSQL *conn, int id_restitution);

// gestion_utilisateurs.c
int is_valid(const char *str);
void suppression_compte(MYSQL *conn, char *username);
void ajout_compte(MYSQL *conn, char *username);
void blocage_compte(MYSQL *conn, char *username);

// gestion_emprunts.c
void emprunter_livre(MYSQL *conn, char *username);
void effectuerEmprunt(MYSQL *conn, char *ISBN, char *username);
int ISBNExiste(MYSQL *conn, char *ISBN);
int obtenirIdExemplaireDisponible(MYSQL *conn, char *ISBN);
void mettreAJourDisponibiliteExemplaire(MYSQL *conn, int id_exemplaire, int disponibilite);
void afficher_emprunts_non_restitues_utilisateur(MYSQL *conn, char *email_utilisateur);

// gestion_exemplaires.c
void ajout_exemplaire(MYSQL *conn);
void mise_a_jour_exemplaire(MYSQL *conn);
void suppression_exemplaire(MYSQL *conn);
void recherche_exemplaire(MYSQL *conn, char *ISBN);

// Consultation_Stat.c
void consultation_stat(MYSQL *conn, int user_type);
void consultation_stat_site(MYSQL *conn, int user_type);
void consultation_stat_3site(MYSQL *conn);

#endif