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
	
	img.r = red;
	img.v = green;
	img.b = blue;
	longueur = red[0].size();
	hauteur = red.size();
}

Image::Image(size_t longueurUser, size_t hauteurUser) {
	img.b.resize(longueurUser, vector<int>(hauteurUser, 0));
	img.v.resize(longueurUser, vector<int>(hauteurUser, 0));
	img.r.resize(longueurUser, vector<int>(hauteurUser, 0));
	longueur = longueurUser;
	hauteur = hauteurUser;
}

Image::Image(Image *image) {
	img = image->img;
	hauteur = image->hauteur;
	longueur = image->longueur;
}

void Image::afficher() {
	for (uint32_t x = 0; x < img.r.size(); ++x) {
		for (uint32_t y = 0; y < img.r[0].size(); ++y)
			cout << img.r[x][y] << ",";
		cout << endl;
	}
	
	cout << endl;

	for (uint32_t x = 0; x < img.v.size(); ++x) {
		for (uint32_t y = 0; y < img.v[0].size(); ++y)
			cout << img.v[x][y] << ",";
		cout << endl;
	}

	cout << endl;
	
	for (uint32_t x = 0; x < img.b.size(); ++x) {
		for (uint32_t y = 0; y < img.b[0].size(); ++y)
			cout << img.b[x][y] << ",";
		cout << endl;
	}

	cout << endl;

}

Image Image::composanteRouge() {
	vector<vector<int>> emptyVec(longueur, vector<int>(hauteur, 0));
	return Image(img.r, emptyVec, emptyVec);
}

bool Image::detection(int r, int v, int b) {
	bool status = false;
	for (uint32_t x = 0; x < img.r.size() && !status; ++x) {
		for (uint32_t y = 0; y < img.r[0].size() && !status; ++y)
			if ((r == img.r[x][y]) && (v == img.v[x][y]) && (b == img.b[x][y])){
				status = true;
			}
	}
	return status;
}

Image Image::niveauxGris() {
	vector<vector<int>> output(longueur, vector<int>(hauteur, 0));
	for (uint32_t x = 0; x < longueur; ++x)
		for (uint32_t y = 0; y < hauteur; ++y)
			output[x][y] = (img.r[x][y] + img.v[x][y] + img.b[x][y])/3;

	return Image(output, output, output);
}

vector<int> Image::histogrammeGris() {
	vector<int> output(longueur * hauteur, 0);
	for (uint32_t y = 0; y < hauteur; ++y)
		for (uint32_t x = 0; x < longueur; ++x)
			output[y * hauteur + x] = (img.r[x][y] + img.v[x][y] + img.b[x][y])/3;

	return output;
}

vector<vector<vector<int>>> Image::histogrammeCouleur() {
	vector<vector<vector<int>>> output(longueur, vector<vector<int>>(hauteur, vector<int>(hauteur, 0)));
	for (uint32_t y = 0; y < hauteur; ++y) {
		for (uint32_t x = 0; x < longueur; ++x) {
			output[y * hauteur + x][0][0] = img.r[x][y];
			output[0][y * hauteur + x][0] = img.v[x][y];
			output[y * hauteur + x][0][0] = img.b[x][y];
		}
	}
	return output;
}

Image Image::noirEtBlanc() {
	vector<vector<int>> output(longueur, vector<int>(hauteur, 0));
	for (uint32_t x = 0; x < longueur; ++x)
		for (uint32_t y = 0; y < hauteur; ++y)
			if((img.r[x][y] + img.v[x][y] + img.b[x][y])/3 < 127){
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
			output_red[x][y] = (img.r[x][y] * (luminosityFactor) > 255 ? 255 : img.r[x][y] * (luminosityFactor));
			output_green[x][y] = (img.v[x][y] * (luminosityFactor) > 255 ? 255 : img.v[x][y] * (luminosityFactor));
			output_blue[x][y] = (img.b[x][y] * (luminosityFactor) > 255 ? 255 : img.b[x][y] * (luminosityFactor));
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
	img.r.resize(height, vector<int>(width, 0));
	img.v.resize(height, vector<int>(width, 0));
	img.b.resize(height, vector<int>(width, 0));

	// ASCII
	if (mMagic == "P3")
	{
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				fichier >> img.r[j][i];
				fichier >> img.v[j][i];
				fichier >> img.b[j][i];
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
	
	for (uint32_t y = 0; y < hauteur; ++y) {
		for (uint32_t x = 0; x < longueur; ++x) {
			fichier << img.r[x][y] << " " << img.v[x][y] << " " << img.b[x][y] << endl;
		}
	}
	fichier.close();
}

Image Image::changeContraste(float contrastFactor) {
	if (contrastFactor < 0)
		contrastFactor = 0;
	Image output(img.r, img.v, img.b);
	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output.img.r[x][y] = (output.img.r[x][y] * contrastFactor > 255 ? 255 : output.img.r[x][y] * contrastFactor);
			output.img.v[x][y] = (output.img.v[x][y] * contrastFactor > 255 ? 255 : output.img.v[x][y] * contrastFactor);
			output.img.b[x][y] = (output.img.b[x][y] * contrastFactor > 255 ? 255 : output.img.b[x][y] * contrastFactor);
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
Image Image::rotationG() {
	//On inverse les dimensions (x*y -> y*x)
	Image output = Image(hauteur, longueur);
	
	for (uint32_t x = 0; x < hauteur; ++x) {
		for (uint32_t y = 0; y < longueur; ++y) {
			output.img.r[y][hauteur - 1 - x] = img.r[x][y];
			output.img.v[y][hauteur - 1 - x] = img.v[x][y];
			output.img.b[y][hauteur - 1 - x] = img.b[x][y];
		}
	}
	return output;
}

Image Image::rotationD() {
	//On inverse les dimensions (x*y -> y*x)
	Image output = Image(hauteur, longueur);
	
	for (uint32_t x = 0; x < hauteur; ++x) {
		for (uint32_t y = 0; y < longueur; ++y) {
			output.img.r[longueur - 1 - y][x] = img.r[x][y];
			output.img.v[longueur - 1 - y][x] = img.v[x][y];
			output.img.b[longueur - 1 - y][x] = img.b[x][y];
		}
	}
	return output;
}

Image Image::retournementH() {
	Image output(longueur, hauteur);

	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output.img.r[x][y] = img.r[longueur - 1 - x][y];
			output.img.v[x][y] = img.v[longueur - 1 - x][y];
			output.img.b[x][y] = img.b[longueur - 1 - x][y];
		}
	}

	return output;
}

Image Image::retournementV() {
	Image output(longueur, hauteur);

	for (uint32_t x = 0; x < longueur; ++x) {
		for (uint32_t y = 0; y < hauteur; ++y) {
			output.img.r[x][y] = img.r[x][hauteur - 1 - y];
			output.img.v[x][y] = img.v[x][hauteur - 1 - y];
			output.img.b[x][y] = img.b[x][hauteur - 1 - y];
		}
	}

	return output;
}
//
Image Image::rognerD(uint32_t nb) {
	if (longueur == 0 || hauteur == 0 || (int)longueur - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(longueur - nb, hauteur);

	for (uint32_t x = 0; x < output.longueur; ++x) {
		for (uint32_t y = 0; y < output.hauteur; ++y) {
			output.img.r[x][y] = img.r[x][y];
			output.img.v[x][y] = img.v[x][y];
			output.img.b[x][y] = img.b[x][y];
		}
	}
	return output;
}

Image Image::rognerG(uint32_t nb) {
	if (longueur == 0 || hauteur == 0 || (int)longueur - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(longueur - nb, hauteur);

	for (uint32_t x = 0; x < output.longueur; ++x) {
		for (uint32_t y = 0; y < output.hauteur; ++y) {
			output.img.r[x][y] = img.r[ x + nb ][y];
			output.img.v[x][y] = img.v[ x + nb ][y];
			output.img.b[x][y] = img.b[ x + nb ][y];
		}
	}
	return output;
}

Image Image::rognerH(uint32_t nb) {
	if (longueur == 0 || hauteur == 0 || (int)hauteur - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(longueur, hauteur - nb);

	for (uint32_t x = 0; x < output.longueur; ++x) {
		for (uint32_t y = 0; y < output.hauteur; ++y) {
			output.img.r[x][y] = img.r[x][ y + nb ];
			output.img.v[x][y] = img.v[x][ y + nb ];
			output.img.b[x][y] = img.b[x][ y + nb ];
		}
	}
	return output;
}

Image Image::rognerB(uint32_t nb) {
	if (longueur == 0 || hauteur == 0 || (int)hauteur - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(longueur, hauteur - nb);

	for (uint32_t x = 0; x < output.longueur; ++x) {
		for (uint32_t y = 0; y < output.hauteur; ++y) {
			output.img.r[x][y] = img.r[x][y];
			output.img.v[x][y] = img.v[x][y];
			output.img.b[x][y] = img.b[x][y];
		}
	}
	return output;
}

uint32_t Image::getLongueur() {
	return longueur;
}
uint32_t Image::getHauteur() {
	return hauteur;
}

Image Image::agrandissement(uint32_t nb) {
	if (longueur == 0 || hauteur == 0 || (int)hauteur - (int)nb <= 0)
		return Image(0, 0);
	
	Image output = Image(longueur * nb, hauteur * nb);
	for (uint32_t x = 0; x < longueur * nb; ++x) {
		for (uint32_t y = 0; y < hauteur * nb; ++y) {
			// Trouver la position correspondante dans l'image d'origine
			uint32_t origX = x / nb;
			uint32_t origY = y / nb;

			// Copier la valeur du pixel le plus proche
			output.img.r[x][y] = img.r[origX][origY];
			output.img.v[x][y] = img.v[origX][origY];
			output.img.b[x][y] = img.b[origX][origY];
		}
	}
	return output;

}

Image Image::retrecissement(uint32_t nb) {
	if (longueur == 0 || hauteur == 0 || nb <= 0)
        return Image(0, 0);

    // Calculer les nouvelles dimensions
    uint32_t nouvelleLongueur = longueur / nb;
    uint32_t nouvelleHauteur = hauteur / nb;

    // Créer une nouvelle image rétrécie
    Image output(nouvelleLongueur, nouvelleHauteur);

    for (uint32_t x = 0; x < nouvelleLongueur; ++x) {
        for (uint32_t y = 0; y < nouvelleHauteur; ++y) {
            // Calculer les indices de début et de fin du bloc dans l'image d'origine
            uint32_t origXStart = x * nb;
            uint32_t origXEnd = origXStart + nb;
            uint32_t origYStart = y * nb;
            uint32_t origYEnd = origYStart + nb;

            // Calculer la moyenne des valeurs dans le bloc
            int redSum = 0, greenSum = 0, blueSum = 0;

            for (uint32_t origX = origXStart; origX < origXEnd; ++origX) {
                for (uint32_t origY = origYStart; origY < origYEnd; ++origY) {
                    redSum += img.r[origX][origY];
                    greenSum += img.v[origX][origY];
                    blueSum += img.b[origX][origY];
                }
            }

            // Remplir la nouvelle image avec la moyenne des valeurs du bloc
            output.img.r[x][y] = redSum / (nb * nb);
            output.img.v[x][y] = greenSum / (nb * nb);
            output.img.b[x][y] = blueSum / (nb * nb);
        }
    }

    return output;
}

Image Image::visionDeuteranopie() {
	Image output = this;
	return output;
}

Image Image::visionProtanopie() {
	Image output = this;
	return output;
}

Image Image::visionTritanopie() {
	Image output = this;
	return output;
}
