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
#include <vector>
#include "constants.hpp"
#include "ui.hpp"
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

class VisualIDK {
	public:
	/**
		* @brief Constructor of VisualIDK, initializes tabs and effects.
	*/
	VisualIDK();
	~VisualIDK();
	ImGuiIO* io;
	ImDrawList* dl;
	UI ui;
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

	
	GLFWwindow* window;

	private:
	

	//TODO
	char fileTempBuffer[50] = "";
	bool noModif = false;

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

	/**
		* @brief Handles loading an image file.
		* @tparam string& file path.
		* @tparam size_t The tab we're loading the image into.
	*/
	void loadFile(const string& filePath, size_t loadingTab);

	thread fileLoadingThread;


};

#endif
