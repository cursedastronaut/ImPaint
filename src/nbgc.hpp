#ifndef _NBGC_
#define _NBGC_
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#define LUMINOSITY_CHANGE 0.1f
#define CONTRAST_CHANGE 1.0f
using namespace std;

struct rgbVec {
	vector<vector<int>> r;
	vector<vector<int>> v;
	vector<vector<int>> b;
};

class Image {
	public:
	//Constructeur de la classe Image
	//Prend trois vecteurs à deux dimensions de nombres entiers, un par canal de couleur.
	Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue);
	//Constructeur de la classe Image
	//Prend un nom de fichier PPM (type P3), et le lit.
	Image(const string& nomFichier);
	//Constructeur de la classe Image
	//Prend deux dimensions, et créé une image noire.
	Image(size_t longueur, size_t hauteur);
	Image(Image *img);

	//Affiche le contenu de chaque vecteur de couleur, séparément.
	void afficher();
	//Retourne vrai s'il y a un pixel de la couleur précisé par les arguments
	//r, v, b, dans l'image chargée.
	bool detection(int r, int v , int b);
	//Renvoie l'image cible, où tout les autres canaux que rouges sont à zéros.
	Image composanteRouge();
	//Renvoie l'image cible, modifié avec 255 nuances de gris.
	Image niveauxGris();
	//Renvoie l'histogramme gris de l'image cible.
	vector<int> histogrammeGris();
	//Renvoie l'histogramme coloré de l'image cible.
	vector<vector<vector<int>>> histogrammeCouleur();
	//Renvoie l'image cible, modifié pour être en noir absolu, ou blanc absolu.
	Image noirEtBlanc();
	//Change la luminosité de l'image cible selon un facteur fourni. (Par défaut, 0.1f)
	Image changeLuminosity(float luminosityFactor = LUMINOSITY_CHANGE);
	//Augmente la luminosité de l'image cible selon un facteur fourni (Par défaut, 1.1f)
	//Formule: facteur = 1 + luminosity (où luminosity est le facteur fourni.)
	Image luminosityUp(float luminosity = LUMINOSITY_CHANGE);
	//Augmente la luminosité de l'image cible selon un facteur fourni (Par défaut, 0.9f)
	//Formule: facteur = 1 - luminosity (où luminosity est le facteur fourni.)
	Image luminosityDown(float luminosity = LUMINOSITY_CHANGE);
	//Change le contraste de l'image cible selon un facteur fourni. (Par défaut, 1.0f)
	//Formule: facteur = 1 - contraste (où contraste est le facteur fourni.)
	Image changeContraste(float contrastFactor = CONTRAST_CHANGE);
	//Change le contraste de l'image cible selon un facteur fourni. (Par défaut, 1.1f)
	//Formule: facteur = 1 - contraste (où contraste est le facteur fourni.)
	Image contrasteUp(float contrastFactor = 0.1f);
	//Change le contraste de l'image cible selon un facteur fourni. (Par défaut, 0.9f)
	//Formule: facteur = 1 - contraste (où contraste est le facteur fourni.)
	Image contrasteDown(float contrastFactor = 0.1f);
	//Écrit le contenu de l'image cible en PPM.
	void ecrire(const string& nomFichier = "output.ppm");
	//Tourne l'image cible à droite
	Image rotationD();
	//Tourne l'image cible à gauche
	Image rotationG();
	//Retourne l'image horizontalement
	Image retournementH();
	//Retourne l'image verticalement
	Image retournementV();
	//Retire nb colonnes à partir de la droite
	Image rognerD(uint32_t nb = 1);
	//Retire nb colonnes à partir de la gauche
	Image rognerG(uint32_t nb = 1);
	//Retire nb lignes à partir du haut
	Image rognerH(uint32_t nb = 1);
	//Retire nb lignes à partir du bas
	Image rognerB(uint32_t nb = 1);
	//Permet d'agrandir l'image cible par un facteur nb (Proche Voisin)
	Image agrandissement(uint32_t nb = 1);
	//Permets de rétrécir l'image cible par un facteur nb
	Image retrecissement(uint32_t nb = 1);
	//Permets d'obtenir la longueur (x)
	uint32_t getLongueur();
	//Permets d'obtenir la largeur (y)
	uint32_t getHauteur();
	Image visionDeuteranopie();
	Image visionProtanopie();
	Image visionTritanopie();

	rgbVec img;
	private:
		uint32_t longueur;
		uint32_t hauteur;
};

#endif //_NBGC_