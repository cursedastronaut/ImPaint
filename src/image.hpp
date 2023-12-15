#ifndef _NBGC_
#define _NBGC_
#include <iostream>
#include "constants.hpp"
#include "filter.hpp"
#include <fstream>
#include <cmath>
#include <cstdint>
#include <sstream>
using namespace std;

class Image {
	public:
		//Image class constructors
		/**
			* @tparam vector<vector<int>> Red channel
			* @tparam vector<vector<int>> Green channel
			* @tparam vector<vector<int>> Blue channel
		*/
		Image(vector<vector<int>> red, vector<vector<int>> green, vector<vector<int>> blue);
		/**
			* @brief Reads a PPM file (type P3) and makes an Image out of it.
			* @tparam string& Path of the PPM file.
		*/
		Image(const string& nomFichier);
		/**
			* @brief Creates a black image.
			* @tparam vector<vector<int>> Red channel
			* @tparam vector<vector<int>> Green channel
			* @tparam vector<vector<int>> Blue channel
		*/
		Image(size_t width, size_t height);
		/**
			* @brief Copies another Image (you can use *this)
			* @tparam Image* Other Image.
		*/
		Image(Image *img);

		/**
			* @brief Couts each color channel's content one by one
		*/
		void displayText();
		//Retourne vrai s'il y a un pixel de la couleur précisé par les arguments
		//r, v, b, dans l'image chargée.
		bool detection(int r, int v , int b);
		/**
			* @brief Returns a copy of the target image, with every channel but the red one muted.
		*/
		Image redCanal();
		/**
			* @brief Returns a copy of the target image, in scales of gray.
		*/
		Image grayScale();
		//Renvoie l'histogramme gris de l'image cible.
		vector<int> histogramGrey();
		//Renvoie l'histogramme coloré de l'image cible.
		vector<vector<vector<int>>> histogramColor();
		/**
			* @brief Returns a copy of the target image, with only pure black or pure white.
		*/
		Image blackWhite();
		//Change la luminosité de l'image cible selon un facteur fourni. (Par défaut, 0.1f)
		/**
			* @brief Returns a copy of the target image, with a modified luminosity.
			* @tparam float Luminosity factor (Defaults to 1.0f)
		*/
		Image changeLuminosity(float luminosityFactor = LUMINOSITY_CHANGE);
		//Augmente la luminosité de l'image cible selon un facteur fourni (Par défaut, 1.1f)
		//Formule: facteur = 1 + luminosity (où luminosity est le facteur fourni.)
		Image luminosityUp(float luminosity = LUMINOSITY_CHANGE);
		//Augmente la luminosité de l'image cible selon un facteur fourni (Par défaut, 0.9f)
		//Formule: facteur = 1 - luminosity (où luminosity est le facteur fourni.)
		Image luminosityDown(float luminosity = LUMINOSITY_CHANGE);
		/**
			* @brief Returns a copy of the target image, with a modified contrast.
			* @tparam float Contrast factor (Defaults to 1.0f)
		*/
		Image changeContraste(float contrastFactor = CONTRAST_CHANGE);
		//Change le contraste de l'image cible selon un facteur fourni. (Par défaut, 1.1f)
		//Formule: facteur = 1 - contraste (où contraste est le facteur fourni.)
		Image contrasteUp(float contrastFactor = 0.1f);
		//Change le contraste de l'image cible selon un facteur fourni. (Par défaut, 0.9f)
		//Formule: facteur = 1 - contraste (où contraste est le facteur fourni.)
		Image contrasteDown(float contrastFactor = 0.1f);
		/**
			* @brief WRites the content of the Image to a specified file.
			* @tparam string& Path to output file.
		*/
		void write(const string& nomFichier = "output.ppm");
		/**
			* @brief Returns a copy of the target image, rotated on the right.
		*/
		Image rotationR();
		/**
			* @brief Returns a copy of the target image, rotated on the left.
		*/
		Image rotationL();
		/**
			* @brief Returns a copy of the target image, rotated horizontally.
		*/
		Image spinH();
		/**
			* @brief Returns a copy of the target image, rotated vertically.
		*/
		Image spinV();
		/**
			* @brief Returns a copy of the target image, with nb colums clipped out from the right.
			* @tparam uint32_t nb, number of colums to remove.
		*/
		Image clipR(uint32_t nb = 1);
		/**
			* @brief Returns a copy of the target image, with nb colums clipped out from the left.
			* @tparam uint32_t nb, number of colums to remove.
		*/
		Image clipL(uint32_t nb = 1);
		/**
			* @brief Returns a copy of the target image, with nb lines clipped out from the top.
			* @tparam uint32_t nb, number of lines to remove.
		*/
		Image clipU(uint32_t nb = 1);
		/**
			* @brief Returns a copy of the target image, with nb lines clipped out from the bottom.
			* @tparam uint32_t nb, number of lines to remove.
		*/
		Image clipD(uint32_t nb = 1);
		/**
			* @brief Returns a copy of the target image, enlarged by a specified factor.
			* @tparam uint32_t Factor by which to enlarge the target Image copy.
		*/
		Image enlarge(uint32_t nb = 1);
		/**
			* @brief Returns a copy of the target image, shrinked by a specified factor.
			* @tparam uint32_t Factor by which to shrink the target Image copy.
		*/
		Image shrink(uint32_t nb = 1);
		/**
			* @brief Returns the Image's width.
		*/
		uint32_t getWidth();
		/**
			* @brief Returns the Image's height.
		*/
		uint32_t getHeight();
		/**
			* @brief Returns a copy of the target image, modified to simulate deuteranopia vision.
		*/
		Image colorblindDeuteranopia();
		/**
			* @brief Returns a copy of the target image, modified to simulate protanopia vision.
		*/
		Image colorblindProtanopia();
		/**
			* @brief Returns a copy of the target image, modified to simulate tritanopia vision.
		*/
		Image colorblindTritanopia();
		/**
			* @brief Returns a copy of the target image, modified to simulate colorblind vision.
		*/
		Image colorblindVision(uint8_t type);
		Image reglageAuto();
		Image reglageAutoGris();
		Image reglageAutoCouleur();

		Image sobelOperator();

		rgbVec img;
	private:
		uint32_t width;
		uint32_t height;
};

#endif //_NBGC_