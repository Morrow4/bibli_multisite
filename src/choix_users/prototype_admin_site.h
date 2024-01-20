#ifndef PROROTYPE_ADMIN_SITE_H
#define PROROTYPE_ADMIN_SITE_H

#include "prototype_inscrit.h"

// Prototype de Reservation_pour_adherent
void Reservation_pour_adherent();

// Prototype de Ajout_adherent
void Ajout_compte();

// Prototype de Suppression_adherent
void Suppression_adherent(const char *login);

// Prototype de Consultation_Stat
void Consultation_Stat();

// Prototype de Ajout_livre
void Ajout_livre();

// Prototype de Suppression_livre
void Suppression_livre();

//Prototype d'emprunt pour un adherent
void Emprunt_pour_adherent();

// Prototype de Fonction_blocage_utilisateur (s'il existe)
void Fonction_blocage_utilisateur();

// Prototype de validation_restitutation
void validation_restitution();

// Prototype de Ajouter_administrateur
void Ajouter_administrateur();

// Prototype de Supprimer_administrateur
void Supprimer_administrateur();

#endif // PROROTYPE_ADMIN_SITE_H
