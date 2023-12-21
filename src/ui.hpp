#pragma once
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <thread>
#include <cmath>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "GLFW/glfw3.h" // Will drag system OpenGL headers
#include <vector>
#include "constants.hpp"
#ifdef _WIN32
	#include "utils/windows.hpp"
	#include <commdlg.h>
#elif __linux__
	#include <imfilebrowser.h>
	#include <clip/clip.h>
#endif

#include "filter.hpp"

struct MainMenuDropdownButton {
	string title;
	bool active = false;
};

struct MainMenuDropdown {
	string title;
	bool active = false;
	vector<MainMenuDropdownButton> buttons;
};

struct boolFuncPointers {
	bool active;
	uint8_t which = 0;
	Image (Image::*func)() = &Image::colorblindTritanopia;
	Image (Image::*funcFloat)(float) = &Image::changeLuminosity;
	Image (Image::*funcUINT32T)(uint32_t) = &Image::enlarge;
	uint8_t argFilter = 0;
	float argFloat = 1.f;
	uint32_t argUint32 = 1;
};

struct ImageTab {
	Image original = Image(0,0);
	Image post = Image(0,0);
	vector<boolFuncPointers> effects;
	vector<boolFuncPointers> effectsCopy;
	std::vector<unsigned char> imageData;
	GLuint textureID;
	int width;
	int height;
	float zoom = 1.f;
	string title = string("Empty");
	bool loading = false;
};

class UI {
	public:
		vector<ImageTab> tabs;

		UI();
		void Update();
		void MenuBar();
		/**
			* @brief Handles the drawing and processing of the Editing Bay. 
		*/
		void Editing();
		/**
			* @brief Handles the drawing and processing of the Toolbar. 
		*/
		void Toolbar();
		/**
			* @brief Handles the drawing and processing of the Error Bay. 
		*/
		void ErrorBay();
		/**
			* @brief Handles the drawing and processing of the tab bar. 
		*/
		void Tabbar();

		void DarkMode();

		void initEffects(size_t tabIndex);
		bool hasSetDefaultSizes = false;
		
		ImVec4 MenuBarSize;
		ImVec4 ImageSize;
		ImVec4 EditingSize;
		ImVec4 ToolbarSize;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		ImGuiIO* io;
		size_t current_tab = 0;
		
		bool darkMode = false;
		bool disableAutoDarkMode = false;
		/**
			* @brief Vector of things to put in the main menu. (File, Edit, Display...)
		*/
		vector <MainMenuDropdown> mainMenu;
		
	private:

};