#include "filter.hpp"
using namespace std;

Filter::Filter(uint8_t type) {
	switch (type)
	{
	case BLURG3:
		action = FILTRE_BLURG3;
		rayon = 1;
		break;
		
	case BLURG5:
		action = FILTRE_BLURG5;
		rayon = 2;
		for (uint32_t x = 0; x < action.size(); ++x) {
			for (uint32_t y = 0; y < action[0].size(); ++y) {
				action[x][y] *= FILTRE_BLURG5_MODIF;
			}
		}
		break;

	case GRADIENTX: action = FILTRE_GRADIENTX;	rayon = 1; break;
	case GRADIENTY:	action = FILTRE_GRADIENTY;	rayon = 1; break;
	case CONTRASTOR:action = FILTRE_CONTRASTOR;	rayon = 1; break;

	default: break;
	}
}

Filter::Filter(vector<vector<float>> actionUser, int rayonUser) {
	action = actionUser;
	rayon = rayonUser;
}

Image Filter::application(Image &original) {
	Image output(original);

	for (size_t x = 0; x < original.getWidth(); ++x) {
		for (size_t y = 0; y < original.getHeight(); ++y) {
			for (size_t channel = 0; channel < 3; ++channel) {
				output.img[channel][x][y] = 0;

				for (int i = -rayon; i <= rayon; ++i) {
					for (int j = -rayon; j <= rayon; ++j) {
						if (x + static_cast<size_t>(i) < original.getWidth() &&
							y + static_cast<size_t>(j) < original.getHeight()) {
							output.img[channel][x][y] +=
								static_cast<int>(action[i + rayon][j + rayon] * original.img[channel][x + static_cast<size_t>(i)][y + static_cast<size_t>(j)]);
						}
					}
				}
			}
		}
	}

	return output;
}
