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

