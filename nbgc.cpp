#include "nbgc.hpp"

Image::Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue, uint32_t longueurUser, uint32_t largeurUser) {
    if (!(red.size() == green.size() == blue.size()))
        throw invalid_argument("ERROR: RGB vectors does not have same size.");
    
    img.red = red;
    img.green = green;
    img.blue = blue;
    longueur = longueurUser;
    largeurUser = largeurUser;
}