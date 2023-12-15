#ifndef _FILTER_
#define _FILTER_
#include "image.hpp"
class Image;
class Filter {
	public:
		Filter(uint8_t type);
		Filter(vector<vector<float>> actionUser, int rayonUser);
		vector<vector<float>> action;
		int rayon;

	Image application(Image &original);
};

#endif