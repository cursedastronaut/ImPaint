#ifndef _MENU_FILES_
#define _MENU_FILES_
#include <string>
//#include <filesystem>
#include "filtre.hpp"

using namespace std;

class Menu {
	public:
		void getFilePath();
		void displayMenu();
		Image loadImage();

	private:
		string filePath;
};

#endif
