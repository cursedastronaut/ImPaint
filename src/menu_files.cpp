#include "menu_files.hpp"

void Menu::getFilePath() {
	while (!(cin >> filePath) || !filesystem::is_regular_file(filePath)) {
		cout << "Le fichier n'existe pas. Veuillez retenter." << endl;
	}
}

void Menu::displayMenu() {
	cout	<< "Menu" << endl
			<< "1- Composante Rouge"
			<< "2- Détection"
			<< "3- Niveaux de Gris"
			<< "4- Noir et Blanc"
			<< "5- Histogramme Gris"
			<< "6- Histogramme Couleur"
			<< "7- Luminosite +"
			<< "8- Luminosite -"

			<< endl;
}