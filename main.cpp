#include "menu_files.hpp"

int main(int argc, char* argv[]) {
	rgbVec test;
	test.red = {{0,0,0,0}, {0,0,0,0}, {0,0,0,255}, {0,0,255,255}};
	test.blue = {{255,255,0,0}, {255,0,0,0}, {0,0,0,0}, {0,0,0,0}};
	test.green = {{0,0,255,255}, {0,255,0,0}, {255,0,0,0}, {0,0,0,0}};
	//Image image = Image(test.red, test.blue, test.green);
	Image image = Image("Couleur.ppm");
	image.afficher();
	image.ecrire("Couleur_GENERATED.ppm");
	return 0;
}