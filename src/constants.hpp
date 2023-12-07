#ifndef _CONSTANTS_
#define _CONSTANTS_

#include <vector>
using namespace std;

const vector<vector<float>> FILTRE_FLOUG3 = {{1.f/16.f, 1.f/8.f, 1.f/16.f}, {1.f/8.f, 1.f/4.f, 1.f/8.f}, {1.f/16.f, 1.f/8.f, 1.f/16.f}};

const vector<vector<float>> FILTRE_FLOUG5 = {
	{01.f, 04.f, 06.f, 04.f, 01.f},
	{04.f, 16.f, 24.f, 16.f, 04.f},
	{06.f, 24.f, 36.f, 24.f, 06.f},
	{04.f, 16.f, 24.f, 16.f, 04.f},
	{01.f, 04.f, 06.f, 04.f, 01.f}
};

const float FILTRE_FLOUG5_MODIF = 1.f/256.f;

const float LUMINOSITY_CHANGE = 0.1f;
const float CONTRAST_CHANGE = 1.0f;

//La simulation du daltonisme ne revient pas qu'à approximer un canal de couleur à zéro.
//Pour obtenir un résultat plus réaliste, mieux vaut utiliser ces valeurs.
//Source:  https://www.color-blindness.com/coblis-color-blindness-simulator/
//Source2: https://www.color-blindness.com/coblis2/js/colorblind.js
const vector<vector<float>> COLORBLIND_TRITAN = {{95.f, 5.f, 0.f}, {0.f, 43.333f, 56.667f}, {0.f, 47.5, 52.5}};
const vector<vector<float>> COLORBLIND_PROTAN = {{56.667f, 43.333f, 0.f}, {55.833f, 44.167f, 0.f}, {0.f, 24.167f, 75.833f}};
const vector<vector<float>> COLORBLIND_DEUTAN = {{62.5f, 37.5f, 0.f}, {70.f, 30.f, 0.f}, {0.f, 30.f, 70.f}};

struct rgbVec {
	vector<vector<int>> r;
	vector<vector<int>> v;
	vector<vector<int>> b;
	vector<vector<int>>& operator[](size_t index) {
		if (index == 0) {
			return r;
		} else if (index == 1) {
			return v;
		} else if (index == 2) {
			return b;
		} else {
			// Handle invalid index (throw an exception, return a default value, etc.)
			// For simplicity, this example throws an exception.
			throw std::out_of_range("Invalid channel index");
		}
	}
};

enum {
	TRITAN,
	PROTAN,
	DEUTAN
};

enum {
	FLOUG3,
	FLOUG5,
};

#endif //_CONSTANTS_