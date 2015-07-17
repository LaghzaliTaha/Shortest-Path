#include <SDL/SDL.h>
#include "itineraire.h"
#include <windows.h>



//projet rèaliser par :  laghzali taha
 //encadrée par : Pr M.E ERRIFI
 //pour toute optimisation ou utilisation de code source veuillez contacter d'abord mrtahalz@gmail.com

using namespace std;
//déclaration de fonctions
void affichagedebase(); //affiche la carte et toutes les liaisons
void placerdepart(int numpoint);//place l'image départ
void placerdestination(int numpoint);//place l'image départ
int clicpoint(int x, int y);//renvoi le numéro d'une ville si les coordonnés x, y lui correspondent (avec une tolérance +- 16 pixels), sinon renvoi 0
void putpixel(int largeur, int longueur, int rouge, int vert, int bleu); //place un pixel reperé et de couleur donnée
int relierpoints(int Xa, int Ya, int Xb, int Yb, int r, int v, int b , int epaisseur); //trace le segment [AB]
int mapause();//gestion des évènement utilisateur (clic souris, clavier...)

//initialisation SDL
SDL_Surface *screen;
SDL_Surface *point;
SDL_Rect pospoint;
SDL_Event event;

int c[100][2]; //matrice de coordonnées
int tt[100][100];//matrice de liaisons
int nombreville = 0;


//fonction principale
int WINAPI WinMain(  HINSTANCE hInstance,
 HINSTANCE hPrevInstance,
LPSTR lpCmdLine,
int nCmdShow)
{
//initialisation
int depart = 1;
int destination = 2;
int clic=1;
int rouge=0;
int vert = 0;
int bleu = 0;
screen= NULL;
point = NULL;
bool continuerok = true;
parcours * itin;


ifstream matrice; //ouverture de la matrice (liaisons des villes) et copie du fichier dans tt
matrice.open("matrice1.txt");
matrice >> nombreville;

for (int z=0; z<nombreville; z++)
{
for (int zz=0; zz<nombreville; zz++)
{
matrice >>tt[z][zz];
}
}


ifstream file; //ouverture des données (coordonnées des villes)
file.open("coord.txt");
for (int z=0; z<nombreville; z++)
{
file >> c[z][0];
file >> c[z][1];
}

destination = nombreville; //soit la destination par défaut est la dernière ville

//initialisation SDL
SDL_Init(SDL_INIT_VIDEO);
screen =  SDL_SetVideoMode(800, 681, 32, SDL_HWSURFACE );
SDL_WM_SetCaption("GPS MAROCkINo WITH TAHA LZ",NULL) ;
SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 250, 250, 250));
relierpoints(10, 10, 90, 90, 135, 135, 135, 1); //test de tracé

affichagedebase(); //affichage carte + liaisons
SDL_Flip(screen); //mise a jour de l'affichage
placerdepart(depart);
placerdestination(destination);
SDL_Flip(screen); //mise a jour de l'affichage

//tracé du parcours initial:

affichagedebase();//fonction affichage carte
placerdestination(destination);
placerdepart(depart);
Itineraire test(depart,destination);//construction de la classe Itineraire
itin = test.ma_main();//capture itineraire calculé
itin=itin->suiv;
while (itin->suiv !=NULL) //dessin de l'itineraire calculé
{
relierpoints(c[itin->ville-1][0], c[itin->ville-1][1], c[itin->suiv->ville-1][0], c[itin->suiv->ville-1][1], 250, 0, 0, 3);
itin=itin->suiv;
}
SDL_Flip(screen); //mise a jour de l'affichage
test.~Itineraire(); //destruction de la classe itineraire



//en attente d'une action de l'utilisateur
while (continuerok == true)
{
clic = mapause();
if (clic == 0) continue; //si le clic ne correspond a aucune ville on recommence la boucle

affichagedebase();//fonction affichage carte

if (clic < 0) //click gauche
depart = -1 * clic;

if (clic > 0) //click gauche
destination = clic;


if (depart != destination)
{
placerdestination(destination);
placerdepart(depart);

Itineraire test(depart,destination);//construction de la classe Itineraire
itin = test.ma_main();//capture itineraire calculé
itin=itin->suiv; //on passe la premiere valeur (qui corresponds à la valeur de la distance du trajet

while (itin->suiv !=NULL) //dessin de l'itineraire calculé
{
relierpoints(c[itin->ville-1][0], c[itin->ville-1][1], c[itin->suiv->ville-1][0], c[itin->suiv->ville-1][1], 250, 0, 0, 3);
itin=itin->suiv;
}

SDL_Flip(screen); //mise a jour de l'affichage
test.~Itineraire(); //destruction de la classe itineraire
}

}
SDL_FreeSurface(point); //destruction
SDL_Quit();
return 0;
}








void affichagedebase()
{
//-----------------------------affichage carte
  SDL_Surface *image1;
  image1 = SDL_LoadBMP("1.bmp");
  SDL_Rect posim;
  posim.x = 0;
  posim.y= 0;



//construction initiale de la carte et des liaisons
  SDL_BlitSurface(point, NULL, screen, &posim);
  SDL_FreeSurface(image1);

for (int cpt=0; cpt < nombreville; cpt ++) //dessiner les liaisons
for (int cpt2=0; cpt2 < nombreville; cpt2 ++)
{
if(tt[cpt][cpt2] !=0) relierpoints(c[cpt][0], c[cpt][1], c[cpt2][0], c[cpt2][1], 125, 125, 125, 4); //dessin des liaisons
}
SDL_Flip(screen); //mise a jour de l'affichage
}


void placerdepart(int numpoint)
{
SDL_Surface *image1;
  image1 = SDL_LoadBMP("depart.bmp");
  SDL_Rect posim;
  posim.x = c[numpoint-1][0];
  posim.y= c[numpoint - 1][1];


  SDL_BlitSurface(point, NULL, screen, &posim);
  SDL_FreeSurface(image1);
   SDL_Flip(screen); //mise a jour de l'affichage
}


void placerdestination(int numpoint)
{
SDL_Surface *image1;
  image1 = SDL_LoadBMP("destination.bmp");
  SDL_Rect posim;
  posim.x = c[numpoint-1][0];
  posim.y= c[numpoint - 1][1];

  SDL_BlitSurface(point, NULL, screen, &posim);
  SDL_FreeSurface(image1);
   SDL_Flip(screen); //mise a jour de l'affichage
}




int clicpoint(int x, int y)
{
int  tolerance = 16;
for (int a=0; a<nombreville; a++)
{
if (x<c[a][0] + tolerance && x > c[a][0] - tolerance) //si x = abscice point a+1, +- 16
if (y<c[a][1] + tolerance && y > c[a][1] - tolerance) //pareil en y
{
return a+1; //point trouvé et numéro du point retourné
}
}
return 0; //point non trouvé

}


void putpixel(int x, int y, int rouge, int vert, int bleu)
{
point =  SDL_CreateRGBSurface(SDL_HWSURFACE, 1, 1, 32, 0, 0, 0, 0);
SDL_FillRect(point, NULL, SDL_MapRGB(point->format, rouge, vert, bleu));
pospoint.x=x;
pospoint.y=y;
SDL_BlitSurface(point, NULL, screen, &pospoint);
SDL_FreeSurface(point);
}


int relierpoints(int Xa, int Ya, int Xb, int Yb, int r, int v, int b , int epaisseur)
{
   if (Xa!=Xb)
{
 int  y;
 if (Xa<Xb)
 {
   for (int x = Xa; x< Xb; x++)
   {y= (-((Xa-x)*(Yb-Ya)/(Xb-Xa))+Ya); putpixel(x, y, r, v, b);  for (int zzz=-epaisseur; zzz<epaisseur +1; zzz++){putpixel(x , y +zzz, r, v, b);putpixel(x +zzz, y +zzz, r, v, b);putpixel(x + zzz , y, r, v, b);}}
}



 if (Xa>Xb)
 {
  for (int x = Xb; x< Xa; x++)
   {y= (-((Xa-x)*(Yb-Ya)/(Xb-Xa))+Ya);putpixel(x , y, r, v, b); for (int zzz=-epaisseur; zzz<epaisseur +1; zzz++){putpixel(x+zzz , y +zzz, r, v, b);putpixel(x , y +zzz, r, v, b);putpixel(x + zzz ,y, r, v, b);}}
  }
}




if (Xa > Xb - 7 && Xa < Xb + 7) //TraitÃ© comme un segment vertical avec une tolÃ©rance +- 5 pixels
{
             int x=Xa;
             if (Ya<Yb)
             {
	     for (int ep = -epaisseur +1; ep < epaisseur; ep ++){
             for (int y = Ya; y< Yb; y++)
             { putpixel(x+ep, y +ep, r, v, b);}}
             }

              if (Ya>Yb)
             {
              for (int ep = -epaisseur+1; ep < epaisseur; ep ++){
             for (int y = Yb; y< Ya; y++)
             { putpixel(x+ep, y+ep, r, v, b);}
             }}
}

return 0;
}




int mapause()
{
    int aaa= 0;
    int continuer = 1;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                exit(0);
		break;

     case SDL_KEYDOWN :
          switch (event.key.keysym.sym)
          {
          case  SDLK_ESCAPE :
             exit(0);
             break;
		}


 case SDL_MOUSEBUTTONUP:

      switch(event.button.button )
            {
                case SDL_BUTTON_RIGHT:

return ( clicpoint(event.button.x, event.button.y )); //si on reconnais les coordonnées d'une ville
break;

                case SDL_BUTTON_LEFT:
return (- clicpoint(event.button.x, event.button.y )); //si on reconnais les coordonnées d'une ville
break;
}
}
}
}

