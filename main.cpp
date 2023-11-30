#include "menu_files.hpp"

int main(int argc, char* argv[]) {
	Image image = Image("ppm_tests/Couleur.ppm");
	image.contrasteDown().ecrire("Couleur_GENERATED.ppm");
	return 0;
}