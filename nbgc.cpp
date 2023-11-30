#include "nbgc.hpp"

Image::Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue) {
    if (
        !(
            red.size() == green.size() &&
            red.size() == blue.size() &&
            green.size() == blue.size()
        ) || !(
            red[0].size() == green[0].size() &&
            red[0].size() == blue[0].size() &&
            green.size() == blue[0].size()
        )
    )
        throw invalid_argument("ERROR: RGB vectors does not have same size.");
    
    img.red = red;
    img.green = green;
    img.blue = blue;
    longueur = red.size();
    largeur = red[0].size();
}

void Image::afficher() {
    for (size_t x = 0; x < img.red.size(); ++x) {
        for (size_t y = 0; y < img.red[0].size(); ++y)
            cout << img.red[x][y] << ",";
        cout << endl;
    }
    
    cout << endl;

    for (size_t x = 0; x < img.green.size(); ++x) {
        for (size_t y = 0; y < img.green[0].size(); ++y)
            cout << img.green[x][y] << ",";
        cout << endl;
    }

    cout << endl;
    
    for (size_t x = 0; x < img.blue.size(); ++x) {
        for (size_t y = 0; y < img.blue[0].size(); ++y)
            cout << img.blue[x][y] << ",";
        cout << endl;
    }

    cout << endl;

}

Image Image::composanteRouge() {
    vector<vector<int>> emptyVec(longueur, vector<int>(largeur, 0));
    return Image(img.red, emptyVec, emptyVec);
}

bool Image::detection(int r, int v, int b) {
    bool status = false;
    for (size_t x = 0; x < img.red.size() && !status; ++x) {
        for (size_t y = 0; y < img.red[0].size() && !status; ++y)
            if ((r == img.red[x][y]) && (v == img.green[x][y]) && (b == img.blue[x][y])){
                status = true;
            }
    }
    return status;
}

Image Image::niveauxGris() {
    vector<vector<int>> output(longueur, vector<int>(largeur, 0));
    for (size_t x = 0; x < longueur; ++x)
        for (size_t y = 0; y < largeur; ++y)
            output[x][y] = (img.red[x][y] + img.green[x][y] + img.blue[x][y])/3;

    return Image(output, output, output);
}

vector<int> Image::histogrammeGris() {
    vector<int> output(longueur * largeur, 0);
    for (size_t y = 0; y < largeur; ++y)
        for (size_t x = 0; x < longueur; ++x)
            output[y * largeur + x] = (img.red[x][y] + img.green[x][y] + img.blue[x][y])/3;

    return output;
}

vector<vector<vector<int>>> Image::histogrammeCouleur() {
    vector<vector<vector<int>>> output(longueur, vector<vector<int>>(largeur, vector<int>(largeur, 0)));
    for (size_t y = 0; y < largeur; ++y) {
        for (size_t x = 0; x < longueur; ++x) {
            output[y * largeur + x][0][0] = img.red[x][y];
            output[0][y * largeur + x][0] = img.green[x][y];
            output[y * largeur + x][0][0] = img.blue[x][y];
        }
    }
    return output;
}
