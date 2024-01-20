// bibliotheque.h

#ifndef PROTOTYPE_INSCRIT_H
#define PROTOTYPE_INSCRIT_H

#include "prototype_invite.h"

// Prototype de Reservation_soimeme_Livre
void Emprunt_soimeme();

// Prototype de Inscri_liste_attente_livre
void Inscri_liste_attente_livre();

// Prototype de Affichage_delai_attente_livre (s'il existe)
void Affichage_delai_attente_livre();

// Prototypes des fonctions utilisées dans Emprunt_soimeme.c
int get_user_type(MYSQL *conn, char *username);
void qui(const char *username);

#endif // PROTOTYPE INSCRIT_H
