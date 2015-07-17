#ifndef ITINERAIRE
#define ITINERAIRE

# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <iostream>
# include <fstream>
struct parcours
{
int ville;
parcours * suiv;
parcours * prec;
};

struct itineraires
{
parcours * p;
itineraires * nxt;
};
using namespace std;
class Itineraire
{
public:




int debut;
int fin;
itineraires * iti;
int taillemax;
int distance_min;
int t[40][40]; //matrice qui repertorie les distances entre les villes (si reliés)
int p[40][40];//matrice p[] constitué du nombre et de la liste des predecesseurs de chaque ville
ofstream  * erreur; //futur fichier qui stockera les erreurs éventuelles (erreur-itineraire.txt)
bool erreurexiste;

Itineraire(int d, int f); //constructeur
parcours *add(parcours *buf, int numville); //ajoute un élément parcours à buf avec comme ville : numville
parcours * newadd(parcours *buf, int numville); //ajoute un élément parcours à buf et créé un autre itineraire en copiant buf, avec comme nouvelle ville : numville
void amat(int i[][40]); //affiche la matrice
int elim(int table[][40], int ville_courante, int nieme_a_eliminer);
/*extrait la nieme valeur de la ligne de la ville courante et ellimine cette valeur dans le reste de la matrice en y appliquant les modifications adequoites afin de la garder vraie
enlève la nième ville de la ville_courante collone afin de la désactiver et réduit les possibilités de liaison à -1*/

void parcourir(int ville_courante, int Pperso[][40], parcours * cheminsuivi, int distance_courante);
//fonction principale de parcours de la carte qui s'appelera elle même (récursivement)

void chargermatrice(int t[][40]); //copie les donnés le fichier matrice1.txt dans t;

void afficher(itineraires * moniti); //affiche le contenu des itineraires

itineraires * listerparcours(); //liste tous les parcours retenus (qui parte du départ et arrivent à la destination

void predecesseur(int t[][40],int p[][40],int n); //génère la matrice p[][]

itineraires * calculdistance(itineraires * chemins, int t[][40]);
/*calcule les distances totales parcourues de chaque itineraire (inutile car la fonction parcourir se charge déjà de cette tache mais peut servir en cas d'amélioration).*/

parcours * meilleur(itineraires * chemins);
//renvoi uniquement de tous les itineraires le meilleur parcours en fonction de la distance minimum.

parcours *  ma_main(); //main - fonction principale de traitement
 ~Itineraire(); //Destructeur
};

#endif


