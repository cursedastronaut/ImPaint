#ifndef _NBGC_
#define _NBGC_
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#define LUMINOSITY_CHANGE 0.1f
using namespace std;

struct rgbVec {
	vector<vector<int>> red;
	vector<vector<int>> green;
	vector<vector<int>> blue;
};

class Image {
	public:
	//Constructeur de la classe Image
	//Prend trois vecteurs à deux dimensions de nombres entiers, un par canal de couleur.
	Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue);
	//Constructeur de la classe Image
	//Prend un nom de fichier PPM (type P3), et le lit.
	Image(const string& nomFichier);

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
	//Renvoie l'image, modifié pour être en noir absolu, ou blanc absolu.
	Image noirEtBlanc();
	//Change la luminosité de l'image selon un facteur fourni. (Par défaut, 0.1f)
	Image changeLuminosity(const float luminosityFactor = LUMINOSITY_CHANGE);
	//Augmente la luminosité de l'image selon un facteur fourni (Par défaut, 1.1f)
	//Formule: facteur = 1 + luminosity (où luminosity est le facteur fourni.)
	Image luminosityUp(const float luminosity = LUMINOSITY_CHANGE);
	//Augmente la luminosité de l'image selon un facteur fourni (Par défaut, 0.9f)
	//Formule: facteur = 1 - luminosity (où luminosity est le facteur fourni.)
	Image luminosityDown(const float luminosity = LUMINOSITY_CHANGE);
	//Écrit le contenu de l'image cible en PPM.
	void ecrire(const string& nomFichier = "output.ppm");
	
	private:
		rgbVec img;
		uint32_t longueur;
		uint32_t largeur;
};

#endif //_NBGC_