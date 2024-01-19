#ifndef UTILITAIRE_H
#define UTILITAIRE_H

#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>

/*
// Structure pour représenter un utilisateur
typedef struct
{
    int ID_Utilisateur;
    char Nom[255];
    char Prenom[255];
    char Email[255];
    char MotDePasse[255];
    char TypeUtilisateur[255];
    int EstChercheur;
    int EstBloque;
    char CommentaireBlocage[255];
} Utilisateur;

// Structure pour représenter un livre
typedef struct
{
    char ISBN[255];
    char Titre[255];
    char Auteur[255];
    char Edition[255];
    char Genre[255];
} Livre;

// Structure pour représenter un exemplaire
typedef struct
{
    int ID_Exemplaire;
    char ISBN[255];
    int Disponibilite;
    char SitePrincipal[255];
    int EstLivrePourChercheur;
} Exemplaire;

// Structure pour représenter un emprunt
typedef struct
{
    int ID_Emprunt;
    int ID_Exemplaire;
    char SiteDeRestitution[255];
    char DateEmprunt[255]; // Vous pouvez utiliser un type de date approprié ici
} Emprunt;

// Structure pour représenter une réservation
typedef struct
{
    int ID_Reservation;
    int ID_Utilisateur;
    char ISBN[255];
    char DateReservation[255]; // Vous pouvez utiliser un type de date approprié ici
    int EstSurListeAttente;
    int PositionListeAttente;
} Reservation;
*/

// Prototypes des fonctions définies dans connect.c
void connect_database();

// Prototypes des fonctions définies dans get_user_group.c
int get_user_type(MYSQL *conn, char *username);

// Prototypes des fonctions définies dans main.c
void choix_admin_general();
void choix_admin_site();
void choix_inscrit();
void choix_invite();

// Prototypes des fonctions définies dans utile.c
int is_valid(const char *str);

#endif
