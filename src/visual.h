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
#include <thread>
#include <cmath>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include "../externals/include/GLFW/glfw3.h" // Will drag system OpenGL headers
#include <vector>
//#include <bits/stdc++.h> 
#include "constants.hpp"
#include "filter.hpp"
#ifdef _WIN32
	#include <windows.h>
	#include <commdlg.h>
#elif __linux__
	#include <imfilebrowser.h>
	#include <clip/clip.h>
#endif
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
	Image (Image::*func)() = &Image::colorblindTritanopia;
	Image (Image::*funcFloat)(float) = &Image::changeLuminosity;
	Image (Image::*funcUINT32T)(uint32_t) = &Image::enlarge;
	uint8_t argFilter = 0;
	float argFloat = 1.f;
	uint32_t argUint32 = 1;
};

struct ImageTab {
	void initEffects();
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

class VisualIDK {
	public:
	/**
		* @brief Constructor of VisualIDK, initializes tabs and effects.
	*/
	VisualIDK();
	~VisualIDK();
	ImGuiIO* io;
	ImDrawList* dl;

	/**
		* @brief Main draw function. Draws the Image.
	*/
	void Draw();
	/**
		* @brief Main update function. 
	*/
	void Update();
	/**
		* @brief Main UI handlind function. Draws UI. 
	*/
	void UI();
	/**
		* @brief Applies the effect to the image if they changed,
		and converts it to an openGL image. It is in its own thread. 
	*/
	void imageRefreshing();

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	GLFWwindow* window;

	private:
	/**
		* @brief Vector of things to put in the main menu. (File, Edit, Display...)
	*/
	vector <MainMenuDropdown> mainMenu;

	//TODO
	char fileTempBuffer[50] = "";
	bool noModif = false;
	size_t current_tab = 0;
	/**
		* @brief Contains every tab, their images, ect... 
	*/
	vector<ImageTab> tabs;

	#ifdef __linux__
	ImGui::FileBrowser fileDialog;
	ImGui::FileBrowser fileDialogSave = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
	#endif

	/**
		* @brief Handles the drawing and processing of the Main Menu Bar (File, Edit, ...)
	*/
	void UIMenuBar();
	/**
		* @brief Handles the drawing and processing of the Editing Bay. 
	*/
	void UIEditing();
	/**
		* @brief Handles the drawing and processing of the Toolbar. 
	*/
	void UIToolbar();
	/**
		* @brief Handles the drawing and processing of the Error Bay. 
	*/
	void UIErrorBay();
	/**
		* @brief Handles the drawing and processing of the tab bar. 
	*/
	void UITabbar();

	/**
		* @brief Handles copying an image from clipboard. 
	*/
	void copyMethod();
	/**
		* @brief Handles pasting an image from clipboard. 
	*/
	void pasteMethod();

	/**
		* @brief Handles opening an image file. 
	*/
	void openFile();
	/**
		* @brief Handles saving an image file. 
	*/
	void saveFile();
	/**
		* @brief Handles applying the effects to an image.
		Should be in imageRefreshing's thread. 
	*/
	void applyEffects();

	void selectTool();

	ImVec4 MenuBarSize;
	ImVec4 ImageSize;
	ImVec4 EditingSize;
	ImVec4 ToolbarSize;
	bool hasSetDefaultSizes = false;
	/**
		* @brief Handles loading an image file.
		* @tparam string& file path.
		* @tparam size_t The tab we're loading the image into.
	*/
	void loadFile(const string& filePath, size_t loadingTab);

	thread fileLoadingThread;

};

#endif
