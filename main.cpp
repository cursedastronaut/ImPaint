#include "nbgc.hpp"

int main(int argc, char* argv[]) {
    rgbVec test;
    test.red = {{0,0,0,0}, {0,0,0,0}, {0,0,0,255}, {0,0,255,255}};
    test.blue = {{255,255,0,0}, {255,0,0,0}, {0,0,0,0}, {0,0,0,0}};
    test.green = {{0,0,255,255}, {0,255,0,0}, {255,0,0,0}, {0,0,0,0}};
    Image image = Image(test.red, test.blue, test.green);
    image.composanteRouge().afficher();
    return 0;
}