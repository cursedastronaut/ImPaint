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
#include <bits/stdc++.h> 
#include "menu_files.hpp"
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
#endif
using namespace std;

class VisualIDK {
	public:
	VisualIDK();
	ImGuiIO* io;
	ImDrawList* dl;

	void Draw();
	void Update();
	void UI();

	Image original = Image("ppm_tests/Ara_ararauna.ppm");
	Image post = Image("ppm_tests/Ara_ararauna.ppm");
	int sizePerPixel = 0;

	private:
	bool noirBlanc = false;
	bool composantRouge = false;
	bool niveauxGris = false;
	bool luminosity = false;
	bool contraste = false;
	bool rotationD = false;
	bool rotationG = false;
	bool retournementH = false;
	bool retournementV = false;
	bool rognerD = false;
	bool rognerG = false;
	bool rognerH = false;
	bool rognerB = false;
	bool dontRefresh = false;

	float luminosityF = 1.0f;
	float contrasteF = 1.0f;
	int pixelSize = 1.0f;

	char fileTempBuffer[50] = "";
};


#endif
