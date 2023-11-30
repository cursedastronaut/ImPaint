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