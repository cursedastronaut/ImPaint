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
	//Constructor of Image class
	Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue);
	Image(const string& nomFichier);
	void afficher();
	bool detection(int r, int v , int b);
	Image composanteRouge();
	
	Image niveauxGris();

	vector<int> histogrammeGris();

	vector<vector<vector<int>>> histogrammeCouleur();
	
	Image noirEtBlanc();

	Image luminosityUp();

	Image luminosityDown();

	void ecrire(const string& nomFichier);
	
	private:
		rgbVec img;
		uint32_t longueur;
		uint32_t largeur;
};

#endif //_NBGC_