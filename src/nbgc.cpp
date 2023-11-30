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
	hauteur = red[0].size();
}

Image::Image(size_t longueurUser, size_t hauteurUser) {
	img.blue.resize(longueurUser, vector<int>(hauteurUser, 0));
	img.green.resize(longueurUser, vector<int>(hauteurUser, 0));
	img.red.resize(longueurUser, vector<int>(hauteurUser, 0));
	longueur = longueurUser;
	hauteur = hauteurUser;
}

void Image::afficher() {
	for (uint32_t x = 0; x < img.red.size(); ++x) {
		for (uint32_t y = 0; y < img.red[0].size(); ++y)
			cout << img.red[x][y] << ",";
		cout << endl;
	}
	
	cout << endl;

	for (uint32_t x = 0; x < img.green.size(); ++x) {
		for (uint32_t y = 0; y < img.green[0].size(); ++y)
			cout << img.green[x][y] << ",";
		cout << endl;
	}

	cout << endl;
	
	for (uint32_t x = 0; x < img.blue.size(); ++x) {
		for (uint32_t y = 0; y < img.blue[0].size(); ++y)
			cout << img.blue[x][y] << ",";
		cout << endl;
	}

	cout << endl;

}

Image Image::composanteRouge() {
	vector<vector<int>> emptyVec(longueur, vector<int>(hauteur, 0));
	return Image(img.red, emptyVec, emptyVec);
}

bool Image::detection(int r, int v, int b) {
	bool status = false;
	for (uint32_t x = 0; x < img.red.size() && !status; ++x) {
		for (uint32_t y = 0; y < img.red[0].size() && !status; ++y)
			if ((r == img.red[x][y]) && (v == img.green[x][y]) && (b == img.blue[x][y])){
				status = true;
			}
	}
	return status;
}

Image Image::niveauxGris() {
	vector<vector<int>> output(longueur, vector<int>(hauteur, 0));
	for (uint32_t x = 0; x < longueur; ++x)
		for (uint32_t y = 0; y < hauteur; ++y)
			output[x][y] = (img.red[x][y] + img.green[x][y] + img.blue[x][y])/3;

	return Image(output, output, output);
}

vector<int> Image::histogrammeGris() {
	vector<int> output(longueur * hauteur, 0);
	for (uint32_t y = 0; y < hauteur; ++y)
		for (uint32_t x = 0; x < longueur; ++x)
			output[y * hauteur + x] = (img.red[x][y] + img.green[x][y] + img.blue[x][y])/3;

	return output;
}

vector<vector<vector<int>>> Image::histogrammeCouleur() {
	vector<vector<vector<int>>> output(longueur, vector<vector<int>>(hauteur, vector<int>(hauteur, 0)));
	for (uint32_t y = 0; y < hauteur; ++y) {
		for (uint32_t x = 0; x < longueur; ++x) {
			output[y * hauteur + x][0][0] = img.red[x][y];
			output[0][y * hauteur + x][0] = img.green[x][y];
			output[y * hauteur + x][0][0] = img.blue[x][y];
		}
	}
	return output;
}

Image Image::noirEtBlanc() {
	vector<vector<int>> output(longueur, vector<int>(hauteur, 0));
	for (uint32_t x = 0; x < longueur; ++x)
		for (uint32_t y = 0; y < hauteur; ++y)
			if((img.red[x][y] + img.green[x][y] + img.blue[x][y])/3 < 127){
				output[x][y] = 0;
			}
			else {
				output[x][y] = 255;
			}

	return Image(output, output, output);

}

Image Image::changeLuminosity(float luminosityFactor) {
	if (luminosityFactor < 0)
		luminosityFactor = 0;
	vector<vector<int>> output_red(longueur, vector<int>(hauteur, 0));
	vector<vector<int>> output_green(longueur, vector<int>(hauteur, 0));
	vector<vector<int>> output_blue(longueur, vector<int>(hauteur, 0));
	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output_red[x][y] = (img.red[x][y] * (luminosityFactor) > 255 ? 255 : img.red[x][y] * (luminosityFactor));
			output_green[x][y] = (img.green[x][y] * (luminosityFactor) > 255 ? 255 : img.green[x][y] * (luminosityFactor));
			output_blue[x][y] = (img.blue[x][y] * (luminosityFactor) > 255 ? 255 : img.blue[x][y] * (luminosityFactor));
		}
	}

	return Image(output_red, output_green, output_blue);
}

Image Image::luminosityUp(float luminosity) {
	return changeLuminosity(1.f + luminosity);
}

Image Image::luminosityDown(float luminosity) {
	return changeLuminosity(1.f - luminosity);
}

Image::Image(const string& nomFichier) {
	ifstream fichier(nomFichier, ifstream::binary);
	if (!fichier.is_open()) 
	{
		cout << "Failed to open " << nomFichier << endl;
		return;
	}
	string mMagic;
	int width, height, maxColor;

	fichier >> mMagic;
	fichier.seekg(1, fichier.cur);
	char c;
	fichier.get(c);
	// Ignore les caractères de chaque ligne de commentaire
	if (c == '#')
	{
		while (c != '\n')
		{
			fichier.get(c);
		}
	}
	else
	{
		fichier.seekg(-1, fichier.cur);
	}
	
	fichier >> width >> height >> maxColor;
	if (maxColor != 255)
	{
		cout << "Failed to read " << nomFichier << endl;
		cout << "Got PPM maximum value: " << maxColor << endl;
		cout << "Maximum pixel has to be 255" << endl;
		return;
	}

	// Resize the vectors to match the image dimensions
	img.red.resize(height, vector<int>(width, 0));
	img.green.resize(height, vector<int>(width, 0));
	img.blue.resize(height, vector<int>(width, 0));

	// ASCII
	if (mMagic == "P3")
	{
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				fichier >> img.red[i][j];
				fichier >> img.green[i][j];
				fichier >> img.blue[i][j];
			}
		}
	}
	else
	{
		cerr << "Format non reconnu." << endl;
		return;
	}
	longueur = width;
	hauteur = height;

}

void Image::ecrire(const string& nomFichier) {
	ofstream fichier(nomFichier, ifstream::binary);
	if (!fichier.is_open()) 
	{
		cout << "Failed to open " << nomFichier << endl;
		return;
	}

	fichier << "P3" << endl << "# Produit par le code de Galaad et Salim pour la S102" << endl
			<< longueur << " " << hauteur << " " << 255 << endl;
	
	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			fichier << img.red[x][y] << " " << img.green[x][y] << " " << img.blue[x][y] << endl;
		}
	}
	fichier.close();
}

Image Image::changeContraste(float contrastFactor) {
	if (contrastFactor < 0)
		contrastFactor = 0;
	Image output(img.red, img.green, img.blue);
	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output.img.red[x][y] = (output.img.red[x][y] * contrastFactor > 255 ? 255 : output.img.red[x][y] * contrastFactor);
			output.img.green[x][y] = (output.img.green[x][y] * contrastFactor > 255 ? 255 : output.img.green[x][y] * contrastFactor);
			output.img.blue[x][y] = (output.img.blue[x][y] * contrastFactor > 255 ? 255 : output.img.blue[x][y] * contrastFactor);
		}
	}

	return output;
}

Image Image::contrasteUp(float contrastFactor) {
	return changeContraste(1.f + contrastFactor);
}

Image Image::contrasteDown(float contrastFactor) {
	return changeContraste(1.f - contrastFactor);
}
Image Image::rotationD() {
	//On inverse les dimensions (x*y -> y*x)
	Image output = Image(hauteur, longueur);
	
	for (uint32_t x = 0; x < hauteur; ++x) {
		for (uint32_t y = 0; y < longueur; ++y) {
			output.img.red[y][hauteur - 1 - x] = img.red[x][y];
			output.img.green[y][hauteur - 1 - x] = img.green[x][y];
			output.img.blue[y][hauteur - 1 - x] = img.blue[x][y];
		}
	}
	return output;
}

Image Image::rotationG() {
	//On inverse les dimensions (x*y -> y*x)
	Image output = Image(hauteur, longueur);
	
	for (uint32_t x = 0; x < hauteur; ++x) {
		for (uint32_t y = 0; y < longueur; ++y) {
			output.img.red[longueur - 1 - y][x] = img.red[x][y];
			output.img.green[longueur - 1 - y][x] = img.green[x][y];
			output.img.blue[longueur - 1 - y][x] = img.blue[x][y];
		}
	}
	return output;
}

Image Image::retournementH() {
	Image output(longueur, hauteur);

	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output.img.red[x][y] = img.red[x][hauteur - 1 - y];
			output.img.green[x][y] = img.green[x][hauteur - 1 - y];
			output.img.blue[x][y] = img.blue[x][hauteur - 1 - y];
		}
	}

	return output;
}

Image Image::retournementV() {
	Image output(longueur, hauteur);

	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output.img.red[x][y] = img.red[longueur - 1 - x][y];
			output.img.green[x][y] = img.green[longueur - 1 - x][y];
			output.img.blue[x][y] = img.blue[longueur - 1 - x][y];
		}
	}

	return output;
}