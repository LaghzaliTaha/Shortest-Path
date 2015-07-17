#include "itineraire.h"

Itineraire::Itineraire(int d, int f) //constructeur de classe et initialisation
{
remove ("erreur-itineraire.txt"); //précaussion si le fichier existait déjà
erreur = new ofstream;
erreur->open("erreur-itineraire.txt");
erreurexiste=false;
distance_min = -1; 
debut=d;
fin=f;
iti=NULL;

}







parcours *Itineraire::add(parcours *buf, int numville)
{
parcours * neww;

if (buf== NULL)//donc parcours est vide et n'existe pas dans itineraires.
{
itineraires * ti;
ti = iti;


buf = new parcours;
buf->ville = 0; //creation de la premiere valeur qui vaudra 1 si le parcours est validé (cad si le chemin permet d'arriver à destination)
buf->prec = NULL;


neww = new parcours;
neww->ville = numville; //ajout de numville
neww->prec=buf; 
neww->suiv = NULL;
buf ->suiv = neww; //racordement avec le précédent
	
	if (iti != NULL) //si itineraire contient au moins un element
		{
		while (ti->nxt != NULL) //on trouve le dernier itineraire
		ti = ti->nxt;

		ti->nxt = new itineraires;
		ti->nxt->p=buf;
		ti->nxt->nxt = NULL;
		}


	if (iti == NULL) //si itineraire est vide
	{iti = new itineraires;
	iti->nxt = NULL;
	iti->p=buf;
	}
		
	
}
else //si buf existe deja (buf non vide)
{ 
parcours * buf2 = buf;
while (buf2->suiv != NULL) //trouver le dernier élèment;
buf2= buf2->suiv; 

neww = new parcours;
buf2->suiv = neww;
neww->ville = numville; //ajout de numville
neww->prec=buf2; 
neww->suiv = NULL;
}
return buf;
}





parcours * Itineraire::newadd(parcours *buf, int numville)
{
if (iti == NULL)//Si iti est vide
{
buf = add(buf, numville);
return buf;
}

else
{
parcours * tmpbuf=buf;
parcours * ret=NULL;

if (buf != NULL)
{
tmpbuf=tmpbuf->suiv;
while (tmpbuf!=NULL)
{ ret=add(ret, tmpbuf->ville);
tmpbuf = tmpbuf->suiv;
}
ret=add(ret, numville);
return ret;
}

if (buf == NULL)
{buf=add(buf, numville);
return buf;}
}
}


void Itineraire::amat(int i[][100]) //affiche la matrice
{cout <<"\n\n";
for (int a=0; a<taillemax; a++) 
{
for (int b=0; b<taillemax; b++)
cout <<i[a][b] << " ";
cout <<"\n";
}
}


int Itineraire::elim(int table[][100], int ville_courante, int nieme_a_eliminer) 
{
if (nieme_a_eliminer > table[ville_courante -1][0])
return 0;
int ville_a_eliminer= table[ville_courante-1][nieme_a_eliminer];

if (ville_a_eliminer == 0)
return 0;
else
for (int a=0; a<taillemax; a++) 
for (int b=1; b<taillemax; b++)
if (table[a][b]==ville_a_eliminer)
{
table[a][0] --; //le nombre de ville reliées est diminué de 1
table[a][b]= table[a][table[a][0]+1]; //on remplacer cette ville par la derniere de la liste
table[a][table[a][0]+1]=0;//on annule la derniere de la liste
//ainsi on se retrouve avec une liste de ville qui relie la ville courante sans la nième
break;
}
return ville_a_eliminer;
}







//fonction principale de parcours de la carte qui s'appelera elle même (récursivement)
void Itineraire::parcourir(int ville_courante, int Pperso[][100], parcours * cheminsuivi, int distance_courante)
{
int nieme_ville=1; //initialisation
int Pperso2[100][100];
int ville_suivante;


if (distance_min < 0 || distance_courante < distance_min) 
//on effectue le parcours uniquement si la distance parcourue ne dépasse pas la distance d'un trajet abouti
{

//copie de la premiere valeur (qui est égale au nombre de villes encore disponibles)
Pperso2[ville_courante-1][0]=Pperso[ville_courante-1][0];

while (Pperso2[ville_courante-1][0] != 0)
{ 
for (int b=0; b<taillemax; b++) 
for (int c=0; c<taillemax; c++)
Pperso2[b][c]=Pperso[b][c];//copie de Pperso dans Perso2

ville_suivante = elim(Pperso2, ville_courante, nieme_ville);

if (ville_suivante == 0)//si chemin fini -> fermeture de la fonction car il n'y a plus de villes reliés disponibles pour ville_courante
return;

parcours * parcoursuivant = NULL;
if (nieme_ville > 0 && ville_suivante != 0 ) //copie et enregistrement nouveau parcours
{
parcoursuivant = newadd(cheminsuivi, ville_courante);
parcourir(ville_suivante, Pperso2, parcoursuivant, distance_courante + t[ville_courante -1][ville_suivante-1]); //recursivité de la même fonction
}


if (ville_suivante== fin) //ville de destination atteinte
{
if (distance_min < 0 || distance_min > distance_courante)
/*ce qui signifie que l'on a trouvé une chemin plus rapide donc on change la valleur de la distance minimale */

distance_min=distance_courante + t[ville_courante -1][ville_suivante-1]; //ajouter la derniere distance a celle-la;
distance_courante = distance_courante + t[ville_courante -1][ville_suivante-1];

parcours * parcoursuivant2 = NULL; //création nouvel itineraire
parcoursuivant2 = newadd (parcoursuivant, ville_courante);
parcoursuivant2 = newadd(parcoursuivant, ville_suivante); 
parcoursuivant2 ->ville = distance_courante;
return; //fermeture fonction vu que le point est atteint
}
nieme_ville++; //on passe à la ville ayant un emplacement suivant dans la liste avant sa modification
}
}
}


void Itineraire::chargermatrice(int t[][100])
{
ifstream fichier("matrice1.txt"); 
if (!fichier)//erreur
{
erreurexiste = true;
*erreur <<"\n Impossible d'ouvrir matrice1.txt.\n";
}


fichier >> taillemax; // lire la taille de la matrice (première info du fichier)

for (int a=0; a<taillemax; a++)
{
for (int b=0; b<taillemax; b++)
fichier >>t[a][b];
}
fichier.close();
}




void Itineraire::afficher(itineraires * moniti) //affiche le contenu de "itineraires"
{
itineraires * b1;
parcours * b2;
b1 = moniti;
b2=b1->p;
while (b1!=NULL)
{
b2=b1->p;
while (b2!=NULL)
 {
   cout <<b2->ville << " - ";
  b2=b2->suiv;
 }
cout <<"\n";
b1=b1->nxt;
}
}


itineraires * Itineraire::listerparcours() //liste les parcours qui ont abouti
{
itineraires * rendu=NULL;
itineraires * boucle=NULL;
itineraires * buffer=iti;

rendu = new itineraires;
rendu->nxt = NULL;
boucle=rendu; //mémoire du début de la liste de pointeur

while (buffer!= NULL)
{
	if (buffer->p->ville !=0)
	{
	boucle->nxt = new itineraires;
	boucle=boucle->nxt;
	boucle->p=buffer->p;
	boucle->nxt = NULL;
	}
	buffer=buffer->nxt;
}
return rendu->nxt;
}


void Itineraire::predecesseur(int t[][100],int p[][100],int n) //génère la matrice p[][]
{ 
int cpt=0; //compteur de villes reliés
for (int a=0; a<n; a++)
{
cpt=0;
	for (int b=0; b<n; b++)
	{
	if(t[a][b] != 0) {cpt++; p[a][0]=cpt; p[a][cpt]=b+1;}
	}
}
}


itineraires * Itineraire::calculdistance(itineraires * chemins, int t[][100])
{
int cpt = 0;//compteur
itineraires * buffer=chemins;
parcours * buf;

while (buffer != NULL)
{
cpt = 0;
buf=buffer->p;
buf=buf->suiv;
	while (buf->suiv !=NULL)
	{
	cpt = cpt + t[buf->ville-1][buf->suiv->ville-1];
	buf=buf->suiv;
	}
buffer->p->ville = cpt;
buffer=buffer->nxt;
}
return chemins;
}


parcours * Itineraire::meilleur(itineraires * chemins) //renvoi le meilleur parcours
{
parcours * minimum=chemins->p;
if (chemins == NULL) return NULL;
else
{
int min=chemins->p->ville;
chemins=chemins->nxt;

while (chemins != NULL)
{
	if(chemins->p->ville <min+1 && chemins->p->ville>0)
	{
	min=chemins->p->ville;
	minimum = chemins->p;
	}
chemins=chemins->nxt;
}
return minimum;
}
}





parcours *  Itineraire::ma_main() //fonction principale de traitement
{ 
parcours * cheminsuivi = NULL;
parcours * rapporterreur = NULL;

chargermatrice(t);
predecesseur(t,p,taillemax);

if( debut > taillemax || debut <1 || fin > taillemax || fin<1)
{erreurexiste = true; *erreur <<"\nle numero de la ville de départ ou d'arrivé est incorrect\n";}


for (int a=0; a<taillemax; a++) 
//on retire de la matrice la ville de départ puisque si l'on repasse par celle-ci cela donnerai des étapes supplémentaires inutiles
for (int b=1; b<taillemax; b++)
if (p[a][b]==debut)
{
p[a][0] --; //le nombre de ville reliées est diminué de 1
p[a][b]= p[a][p[a][0]+1]; //on remplace cette ville par la derniere de la liste
p[a][p[a][0]+1]=0;//on annule la derniere de la liste
//ainsi on se retrouve avec une liste de ville qui relie la ville courante sans la ville de début
break;
}


parcourir(debut, p, cheminsuivi, 0); //appel de la fonction de parcours
//iti contiendra tous les itinéraires

//calculdistance(listerparcours(), t); 
//calculer les distances des parcours choisis inutile car la nouvelle version permet de calculer en direct dans parcourir()

parcours * thebest = new parcours; //création de Itineraire qui stockera
thebest=meilleur(listerparcours()); //pointeur sur le meilleur parcours
return thebest; //retourne le meilleur itineraire
}


Itineraire::~Itineraire() //destructeur
{
erreur->close(); //fermeture du fichier erreur
if( erreurexiste == false)//si pas d'erreur suppression du fichier
remove("erreur-itineraire.txt");
}


