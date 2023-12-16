#include "image.hpp"
#include "filter.hpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <gpt.hpp>
using namespace std;

Image::Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue) {
	if (
		!(
			red.size() == green.size() &&
			red.size() == blue.size() &&
			green.size() == blue.size()
		) || !(
			red[0].size() == green[0].size() &&
			red[0].size() == blue[0].size() &&
			green[0].size() == blue[0].size()
		)
	)
		throw invalid_argument("ERROR: RGB vectors does not have same size.");
	
	img.r = red;
	img.v = green;
	img.b = blue;
	width = red.size();
	height = red[0].size();
}

Image::Image(size_t widthUser, size_t heightUser) {
	if (widthUser != 0 && heightUser != 0) {
		img.b.resize(widthUser, vector<int>(heightUser, 0));
		img.v.resize(widthUser, vector<int>(heightUser, 0));
		img.r.resize(widthUser, vector<int>(heightUser, 0));
	} else {
		img.r.clear();
		img.v.clear();
		img.b.clear();
	}
	width = widthUser;
	height = heightUser;
	error = "No error";
}

Image::Image(const string& nomFichier) {
	if (!(GPT::str::endsWith(nomFichier, ".ppm"))) {
		int x, y, comp;
		stbi_uc *pixels = stbi_load(nomFichier.c_str(), &x, &y, &comp, 3);
		for (int ix = 0; ix < x; ix++) {
			std::vector<int> columnR;
			std::vector<int> columnG;
			std::vector<int> columnB;
			for (int iy = 0; iy < y; iy++) {
				int index = 3 * (iy * x + ix);
				columnR.push_back(pixels[index+0]);
				columnG.push_back(pixels[index+1]);
				columnB.push_back(pixels[index+2]);
			}
			
			img.r.push_back(columnR);
			img.v.push_back(columnG);
			img.b.push_back(columnB);
		}

		width = x;
		height = y;

		stbi_image_free(pixels);

	} else {
		//IF FILE IS PPM
		ifstream fichier(nomFichier, ifstream::binary);
		if (!fichier.is_open()) 
		{
			*this = Image(0,0);
			error = "Failed to open ";
			error += nomFichier;
			return;
		}
		string mMagic;
		int maxColor;

		fichier >> mMagic;
		fichier.seekg(1, fichier.cur);
		char c;
		fichier.get(c);
		//Ignores every line starting with a #
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
			*this = Image(0,0);
			error = "ERROR: PPM maximum value is ";
			error += to_string(maxColor);
			error += ". It must be 255.";
			return;
		}

		// Resize the vectors to match the image dimensions
		img.r.resize(width, vector<int>(height, 0));
		img.v.resize(width, vector<int>(height, 0));
		img.b.resize(width, vector<int>(height, 0));

		// ASCII
		if (mMagic == "P3")
		{
			cout << nomFichier << " loading (in %): ";
			int quarter = height / 4;
			for (uint32_t i = 0, j = 1; i < height && !fichier.eof(); ++i) {
				if (i == quarter * j-1) {
					cout << 100/4 * j << (100/4*j == 100 ? "" : ", ");
					j++;
				}
				for (uint32_t j = 0; j < width && !fichier.eof(); ++j) {
					fichier >> img.r[j][i];
					fichier >> img.v[j][i];
					fichier >> img.b[j][i];
				}
			}
		}
		else
		{
			*this = Image(0,0);
			error = "ERROR: PPM format is not P3.";
			return;
		}
		cout << endl;
	}
}

Image::Image(Image *image) {
	img = image->img;
	height = image->height;
	width = image->width;
}

void Image::displayText() {
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

Image Image::redCanal() {
	emptyError;
	vector<vector<int>> emptyVec(width, vector<int>(height, 0));
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

Image Image::grayScale() {
	emptyError;
	vector<vector<int>> output(width, vector<int>(height, 0));
	for (uint32_t x = 0; x < width; ++x)
		for (uint32_t y = 0; y < height; ++y)
			output[x][y] = (img.r[x][y] + img.v[x][y] + img.b[x][y])/3;

	return Image(output, output, output);
}

vector<int> Image::histogramGrey() {
	vector<int> output(width * height, 0);
	for (uint32_t y = 0; y < height; ++y)
		for (uint32_t x = 0; x < width; ++x)
			output[y * height + x] = (img.r[x][y] + img.v[x][y] + img.b[x][y])/3;

	return output;
}

vector<vector<vector<int>>> Image::histogramColor() {
	vector<vector<vector<int>>> output(width, vector<vector<int>>(height, vector<int>(height, 0)));
	for (uint32_t y = 0; y < height; ++y) {
		for (uint32_t x = 0; x < width; ++x) {
			output[y * height + x][0][0] = img.r[x][y];
			output[0][y * height + x][0] = img.v[x][y];
			output[y * height + x][0][0] = img.b[x][y];
		}
	}
	return output;
}

Image Image::blackWhite() {
	emptyError;
	vector<vector<int>> output(width, vector<int>(height, 0));
	for (uint32_t x = 0; x < width; ++x)
		for (uint32_t y = 0; y < height; ++y)
			if((img.r[x][y] + img.v[x][y] + img.b[x][y])/3 < 127){
				output[x][y] = 0;
			}
			else {
				output[x][y] = 255;
			}

	return Image(output, output, output);

}

Image Image::changeLuminosity(float luminosityFactor) {
	emptyError;
	if (luminosityFactor < 0)
		luminosityFactor = 0;
	if (luminosityFactor == 1)
		return this;
	vector<vector<int>> output_red(width, vector<int>(height, 0));
	vector<vector<int>> output_green(width, vector<int>(height, 0));
	vector<vector<int>> output_blue(width, vector<int>(height, 0));
	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
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

void rgbVecToUCharVec(vector<unsigned char> &data, rgbVec &img, int height, int width) {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			data.push_back(static_cast<unsigned char>(img.r[x][y]));
			data.push_back(static_cast<unsigned char>(img.v[x][y]));
			data.push_back(static_cast<unsigned char>(img.b[x][y]));
		}
	}
}

void Image::write(const string& nomFichier) {
	if (height == 0 || width == 0) {
		error = "ERROR: Cannot write an empty image. (Height or width equals zero.)";
		return;
	}

	if (GPT::str::endsWith(nomFichier, "png")) {
		vector<unsigned char> imageData;
		rgbVecToUCharVec(imageData, img, height, width);
		stbi_write_png(nomFichier.c_str(), width, height, 3, imageData.data(), width * 3);

	} else if (GPT::str::endsWith(nomFichier, "bmp")) {
		
		vector<unsigned char> imageData;
		rgbVecToUCharVec(imageData, img, height, width);
		stbi_write_bmp(nomFichier.c_str(), width, height, 3, imageData.data());
	} else if (GPT::str::endsWith(nomFichier, "jpg") || GPT::str::endsWith(nomFichier, "jpeg")) {

		vector<unsigned char> imageData;
		rgbVecToUCharVec(imageData, img, height, width);
		stbi_write_jpg(nomFichier.c_str(), width, height, 3, imageData.data(), 100);

	} else if (GPT::str::endsWith(nomFichier, "ppm")) {
		ofstream fichier(nomFichier, ifstream::binary);
		if (!fichier.is_open()) 
		{
			cout << "Failed to open " << nomFichier << endl;
			return;
		}
		fichier << "P3" << endl << "# Made by ImPaint (https://github.com/cursedastronaut/ImPaint)" << endl
				<< width << " " << height << " " << 255 << endl;
		
		int dizPourcent = height / 10 - 1; 
		cout << nomFichier << " ecriture (en %): ";
		for (uint32_t y = 0, j = 1; y < height; ++y) {
			if (y == dizPourcent * j) {
				cout << 100/10 * j << (100/10*j == 100 ? "" : ", ");
				++j;
			}
			
			for (uint32_t x = 0; x < width; ++x) {
				fichier << img.r[x][y] << " " << img.v[x][y] << " " << img.b[x][y] << endl;
			}
		}
		fichier.close();
		cout << endl;
	} else {
		cout << "Unknown extension, could not write." << endl;
	}
}

Image Image::changeContraste(float contrastFactor) {
	emptyError;
	if (contrastFactor < 0)
		contrastFactor = 0;
	if (contrastFactor == 1)
		return this;

	Image output(this);
	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			output.img.r[x][y] = (output.img.r[x][y] * contrastFactor > 255 ? 255 : output.img.r[x][y] * contrastFactor);
			output.img.v[x][y] = (output.img.v[x][y] * contrastFactor > 255 ? 255 : output.img.v[x][y] * contrastFactor);
			output.img.b[x][y] = (output.img.b[x][y] * contrastFactor > 255 ? 255 : output.img.b[x][y] * contrastFactor);
		}
	}

	return output;
}

Image Image::contrasteUp(float contrastFactor) {
	if (contrastFactor < 0)
		contrastFactor = 0;
	if (contrastFactor == 1)
		return this;

	Image output(this);
	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {

			output.img.r[x][y] = ((output.img.r[x][y] - 128)*contrastFactor + 128);  //Red

			if (output.img.r[x][y] > 255){
				output.img.r[x][y] = 255;
			}
			else if (output.img.r[x][y] < 0){
				output.img.r[x][y] = 0;
			}
			output.img.v[x][y] = ((output.img.v[x][y] - 128)*contrastFactor + 128);  //GReen

			if (output.img.v[x][y] > 255){
				output.img.v[x][y] = 255;
			}
			else if (output.img.v[x][y] < 0){
				output.img.v[x][y] = 0;

			}
			output.img.b[x][y] = ((output.img.b[x][y] - 128)*contrastFactor + 128);  //Blue
			if (output.img.b[x][y] > 255){
				output.img.b[x][y] = 255;
			}
			else if (output.img.b[x][y] < 0){
				output.img.b[x][y] = 0;

			}
		}
	}

	return output;
}

Image Image::contrasteDown(float contrastFactor) {   //TODO
	if (contrastFactor < 0)
		contrastFactor = 0;
	if (contrastFactor == 1)
		return this;

	Image output(this);
	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			output.img.r[x][y] = ((output.img.r[x][y] - 128) / contrastFactor) + 128;  // ROUGE

			if (output.img.r[x][y] > 255) {
				output.img.r[x][y] = 255;
			}
			else if (output.img.r[x][y] < 0) {
				output.img.r[x][y] = 0;
			}

			output.img.v[x][y] = ((output.img.v[x][y] - 128) / contrastFactor) + 128;  // VERT

			if (output.img.v[x][y] > 255) {
				output.img.v[x][y] = 255;
			}
			else if (output.img.v[x][y] < 0) {
				output.img.v[x][y] = 0;
			}

			output.img.b[x][y] = ((output.img.b[x][y] - 128) / contrastFactor) + 128;  // BLEU

			if (output.img.b[x][y] > 255) {
				output.img.b[x][y] = 255;
			}
			else if (output.img.b[x][y] < 0) {
				output.img.b[x][y] = 0;
			}
		}
	}

	return output;
}

Image Image::rotationL() {
	emptyError;
	//Reverse the width and height (x*y -> y*x)
	Image output = Image(height, width);
	
	for (uint32_t x = 0; x < height; ++x) {
		for (uint32_t y = 0; y < width; ++y) {
			output.img.r[y][height - 1 - x] = img.r[x][y];
			output.img.v[y][height - 1 - x] = img.v[x][y];
			output.img.b[y][height - 1 - x] = img.b[x][y];
		}
	}
	return output;
}

Image Image::rotationR() {
	emptyError;
	//Reverse the width and height (x*y -> y*x)
	Image output = Image(height, width);
	
	for (uint32_t x = 0; x < height; ++x) {
		for (uint32_t y = 0; y < width; ++y) {
			output.img.r[width - 1 - y][x] = img.r[x][y];
			output.img.v[width - 1 - y][x] = img.v[x][y];
			output.img.b[width - 1 - y][x] = img.b[x][y];
		}
	}
	return output;
}

Image Image::spinH() {
	emptyError;
	Image output(width, height);

	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			output.img.r[x][y] = img.r[width - 1 - x][y];
			output.img.v[x][y] = img.v[width - 1 - x][y];
			output.img.b[x][y] = img.b[width - 1 - x][y];
		}
	}

	return output;
}

Image Image::spinV() {
	emptyError;
	Image output(width, height);

	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			output.img.r[x][y] = img.r[x][height - 1 - y];
			output.img.v[x][y] = img.v[x][height - 1 - y];
			output.img.b[x][y] = img.b[x][height - 1 - y];
		}
	}

	return output;
}

Image Image::clipR(uint32_t nb) {
	emptyError;
	if (width == 0 || height == 0 || (int)width - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(width - nb, height);

	for (uint32_t x = 0; x < output.width; ++x) {
		for (uint32_t y = 0; y < output.height; ++y) {
			output.img.r[x][y] = img.r[x][y];
			output.img.v[x][y] = img.v[x][y];
			output.img.b[x][y] = img.b[x][y];
		}
	}
	return output;
}

Image Image::clipL(uint32_t nb) {
	emptyError;
	if (width == 0 || height == 0 || (int)width - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(width - nb, height);

	for (uint32_t x = 0; x < output.width; ++x) {
		for (uint32_t y = 0; y < output.height; ++y) {
			output.img.r[x][y] = img.r[ x + nb ][y];
			output.img.v[x][y] = img.v[ x + nb ][y];
			output.img.b[x][y] = img.b[ x + nb ][y];
		}
	}
	return output;
}

Image Image::clipU(uint32_t nb) {
	emptyError;
	if (width == 0 || height == 0 || (int)height - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(width, height - nb);

	for (uint32_t x = 0; x < output.width; ++x) {
		for (uint32_t y = 0; y < output.height; ++y) {
			output.img.r[x][y] = img.r[x][ y + nb ];
			output.img.v[x][y] = img.v[x][ y + nb ];
			output.img.b[x][y] = img.b[x][ y + nb ];
		}
	}
	return output;
}

Image Image::clipD(uint32_t nb) {
	emptyError;
	if (width == 0 || height == 0 || (int)height - (int)nb <= 0)
		return Image(0, 0);

	// Create a new image with nb less column
	Image output(width, height - nb);

	for (uint32_t x = 0; x < output.width; ++x) {
		for (uint32_t y = 0; y < output.height; ++y) {
			output.img.r[x][y] = img.r[x][y];
			output.img.v[x][y] = img.v[x][y];
			output.img.b[x][y] = img.b[x][y];
		}
	}
	return output;
}

uint32_t Image::getWidth() {
	return width;
}

uint32_t Image::getHeight() {
	return height;
}

Image Image::enlarge(uint32_t nb) {
	emptyError;
	if (width == 0 || height == 0 || nb <= 0) {

		return Image(0, 0);
	}
	
	Image output = Image(width * nb, height * nb);
	for (uint32_t x = 0; x < width * nb; ++x) {
		for (uint32_t y = 0; y < height * nb; ++y) {
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

Image Image::shrink(uint32_t nb) {
	emptyError;
	if (width == 0 || height == 0 || nb <= 0)
		return Image(0, 0);

	// Calculer les nouvelles dimensions
	uint32_t nouvelleLongueur = width / nb;
	uint32_t nouvelleHauteur = height / nb;

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

Image Image::colorblindVision(uint8_t type) {
	emptyError;
	vector<vector<float>> colorMatrix;
	switch (type)
	{
		case TRITAN:	colorMatrix = COLORBLIND_TRITAN;	break;
		case PROTAN:	colorMatrix = COLORBLIND_PROTAN;	break;
		case DEUTAN:	colorMatrix = COLORBLIND_DEUTAN;	break;
		default:		break;
	}

	Image output(this);
	for (uint32_t x = 0; x < width; ++x) {
		for (uint32_t y = 0; y < height; ++y) {
			// Calculate tritanopia simulation
			int newR = static_cast<int>((float)img.r[x][y] * colorMatrix[0][0] / 100.f + (float)img.v[x][y] * colorMatrix[0][1] / 100.f + (float)img.b[x][y] * colorMatrix[0][2] / 100.f);
			int newV = static_cast<int>((float)img.r[x][y] * colorMatrix[1][0] / 100.f + (float)img.v[x][y] * colorMatrix[1][1] / 100.f + (float)img.b[x][y] * colorMatrix[1][2] / 100.f);
			int newB = static_cast<int>((float)img.r[x][y] * colorMatrix[2][0] / 100.f + (float)img.v[x][y] * colorMatrix[2][1] / 100.f + (float)img.b[x][y] * colorMatrix[2][2] / 100.f);

			// Set the new values to the output image
			output.img.r[x][y] = std::min(255, std::max(0, newR));
			output.img.v[x][y] = std::min(255, std::max(0, newV));
			output.img.b[x][y] = std::min(255, std::max(0, newB));
		}
	}

	return output;
}

Image Image::colorblindDeuteranopia() {
	return colorblindVision(DEUTAN);
}

Image Image::colorblindProtanopia() {
	return colorblindVision(PROTAN);
}

Image Image::colorblindTritanopia() {
	return colorblindVision(TRITAN);
}

Image Image::sobelOperator() {
	emptyError;
	Image output = this;
	vector<vector<float>> gradient = FILTRE_GRADIENTX;
	Filter gradientX(GRADIENTX);
	Filter gradientY(GRADIENTY);
	if (!(gradientX.action.size() == gradientY.action.size() && gradientX.action[0].size() == gradientY.action[0].size()))
		return Image(0,0);
	for (uint32_t x = 0; x < gradientX.action.size(); ++x)
			for (uint32_t y = 0; y < gradientX.action[0].size(); ++y)
				gradient[x][y] = sqrtf(powf(gradientX.action[x][y], 2) + powf(gradientY.action[x][y], 2));
	Filter gradientFiltre(gradient, 1);
	
	return gradientFiltre.application(output);
}

Image Image::reglageAuto() {
	return Image(*this);
}
	

Image Image::reglageAutoGris() {
	return Image(*this);
}

Image Image::reglageAutoCouleur() {
	return Image(*this);
}

string & Image::getError() {
	return error;
}