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
	for (uint32_t x = 0; x < original.getLongueur(); ++x) {
		for (uint32_t y = 0; y < original.getHauteur(); ++y) {
			output.img.r[x][y] = (
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[0][0] * original.img.r[x-1][y-1]) ) +
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[0][1] * original.img.r[x  ][y-1]) ) +
				( x == original.getLongueur() - 1 || y == 0 ? 0 : static_cast<int>(action[0][2] * original.img.r[x+1][y-1]) ) +

				( x == 0 || y == 0 ? 0 : static_cast<int>(action[1][0] * original.img.r[x-1][y  ]) ) +
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[1][1] * original.img.r[x  ][y  ]) ) +
				( x == original.getLongueur() - 1 ? 0 : static_cast<int>(action[1][2] * original.img.r[x+1][y  ]) ) +

				( x == 0 || y == original.getHauteur() - 1 ? 0 : static_cast<int>(action[2][0] * original.img.r[x-1][y+1]) ) +
				( x == 0 ? 0 : static_cast<int>(action[2][1] * original.img.r[x  ][y+1]) ) +
				( x == original.getLongueur() - 1 || y == original.getHauteur() - 1 ? 0 : static_cast<int>(action[2][2] * original.img.r[x+1][y+1]) )
			);

			output.img.v[x][y] = (
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[0][0] * original.img.v[x-1][y-1]) ) +
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[0][1] * original.img.v[x  ][y-1]) ) +
				( x == original.getLongueur() - 1 || y == 0 ? 0 : static_cast<int>(action[0][2] * original.img.v[x+1][y-1]) ) +

				( x == 0 || y == 0 ? 0 : static_cast<int>(action[1][0] * original.img.v[x-1][y  ]) ) +
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[1][1] * original.img.v[x  ][y  ]) ) +
				( x == original.getLongueur() - 1 ? 0 : static_cast<int>(action[1][2] * original.img.v[x+1][y  ]) ) +

				( x == 0 || y == original.getHauteur() - 1 ? 0 : static_cast<int>(action[2][0] * original.img.v[x-1][y+1]) ) +
				( x == 0 ? 0 : static_cast<int>(action[2][1] * original.img.v[x  ][y+1]) ) +
				( x == original.getLongueur() - 1 || y == original.getHauteur() - 1 ? 0 : static_cast<int>(action[2][2] * original.img.v[x+1][y+1]) )
			);

			output.img.b[x][y] = (
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[0][0] * original.img.b[x-1][y-1]) ) +
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[0][1] * original.img.b[x  ][y-1]) ) +
				( x == original.getLongueur() - 1 || y == 0 ? 0 : static_cast<int>(action[0][2] * original.img.b[x+1][y-1]) ) +

				( x == 0 || y == 0 ? 0 : static_cast<int>(action[1][0] * original.img.b[x-1][y  ]) ) +
				( x == 0 || y == 0 ? 0 : static_cast<int>(action[1][1] * original.img.b[x  ][y  ]) ) +
				( x == original.getLongueur() - 1 ? 0 : static_cast<int>(action[1][2] * original.img.b[x+1][y  ]) ) +

				( x == 0 || y == original.getHauteur() - 1 ? 0 : static_cast<int>(action[2][0] * original.img.b[x-1][y+1]) ) +
				( x == 0 ? 0 : static_cast<int>(action[2][1] * original.img.b[x  ][y+1]) ) +
				( x == original.getLongueur() - 1 || y == original.getHauteur() - 1 ? 0 : static_cast<int>(action[2][2] * original.img.b[x+1][y+1]) )
			);
		}
	}

	return output;
}