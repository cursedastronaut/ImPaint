#ifndef _NBGC_
#define _NBGC_
#include <iostream>
#include <vector>

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
    void afficher();
    bool detection(int r, int v , int b);
    Image composanteRouge();
    
    Image Image::niveauxGris();

    vector<int> Image::histogrammeGris();

    vector<vector<vector<int>>> Image::histogrammeCouleur();
    private:
        rgbVec img;
        uint32_t longueur;
        uint32_t largeur;
};

#endif //_NBGC_