#ifndef _FILTER_
#define _FILTER_
#include "nbgc.hpp"

class Filtre {
	public:
		Filtre(uint8_t type);
		Filtre(vector<vector<float>> actionUser, int rayonUser);
		vector<vector<float>> action;
		int rayon;

	Image application(Image &original);
};

#endif