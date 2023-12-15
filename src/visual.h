#pragma once
#ifndef __SNAKE_CLASS__
#define __SNAKE_CLASS__
#include "../externals/include/imgui/imgui.h"
#include "../externals/include/imgui/backends/imgui_impl_glfw.h"
#include "../externals/include/imgui/backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include <cmath>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "../externals/include/GLFW/glfw3.h" // Will drag system OpenGL headers
#include <vector>
//#include <bits/stdc++.h> 
#include "menu_files.hpp"
#ifdef _WIN32
	#include <windows.h>
	#include <commdlg.h>
#endif
#include <imfilebrowser.h>
/*
#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif*/
using namespace std;

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
	Image (Image::*func)() = &Image::visionTritanopie;
	Image (Image::*funcFloat)(float) = &Image::changeLuminosity;
	Image (Image::*funcUINT32T)(uint32_t) = &Image::agrandissement;
	uint8_t argFilter = 0;
	float argFloat = 1.f;
	uint32_t argUint32 = 1;
};

class VisualIDK {
	public:
	VisualIDK();
	ImGuiIO* io;
	ImDrawList* dl;

	void Draw();
	void Update();
	void UI();

	Image original = Image("ppm_tests/Ara_ararauna.ppm");
	Image post = Image(0,0);
	int sizePerPixel = 0;

	private:
	

	vector<boolFuncPointers> effects;
	//float luminosityF = 1.0f;
	//float contrasteF = 1.0f;
	//int agrandissementV = 1;
	//int retrecissementV = 1;
	vector <MainMenuDropdown> mainMenu;

	char fileTempBuffer[50] = "";
	bool noModif = false;
	#ifndef USE_DUMB_DRAW
	std::vector<unsigned char> imageData;
	GLuint textureID;
	int width;
	int height;
	#endif //USE_DUMB_DRAW

	#ifdef __linux__
	ImGui::FileBrowser fileDialog;
	ImGui::FileBrowser fileDialogSave = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
	#endif
};

#endif
