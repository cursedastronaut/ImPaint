#include "filtre.hpp"
Filtre::Filtre(uint8_t type) {
	switch (type)
	{
	case FLOUG3:
		action = FILTRE_FLOUG3;
		rayon = 1;
		break;
		
	case FLOUG5:
		action = FILTRE_FLOUG5;
		rayon = 2;
		for (uint32_t x = 0; x < action.size(); ++x) {
			for (uint32_t y = 0; y < action[0].size(); ++y) {
				action[x][y] *= FILTRE_FLOUG5_MODIF;
			}
		}
		break;

	default:
		break;
	}
}

Image Filtre::application(Image &original) {
	Image output(original);

	for (size_t x = 0; x < original.getLongueur(); ++x) {
		for (size_t y = 0; y < original.getHauteur(); ++y) {
			for (size_t channel = 0; channel < 3; ++channel) {
				output.img[channel][x][y] = 0;

				for (int i = -rayon; i <= rayon; ++i) {
					for (int j = -rayon; j <= rayon; ++j) {
						if (x + static_cast<size_t>(i) < original.getLongueur() &&
							y + static_cast<size_t>(j) < original.getHauteur()) {
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
