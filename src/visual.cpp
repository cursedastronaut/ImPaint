#include "visual.h"
 
void VisualIDK::loadFile(const string& filePath, size_t loadingTab) {
	if (!ui.tabs[loadingTab].loading)
		return;
	ui.tabs[loadingTab].original = Image(filePath);
	ui.tabs[loadingTab].title = filePath;
	ui.tabs[loadingTab].post = ui.tabs[loadingTab].original;
	ui.tabs[loadingTab].loading = false;
	noModif = false;
}



VisualIDK::VisualIDK() {
	#ifdef __linux__
	fileDialog.SetTitle("Open...");
	fileDialog.SetTypeFilters({".ppm", ".png", ".bmp", ".jpg", ".jpeg"});
	
	fileDialogSave.SetTitle("Save...");
	fileDialogSave.SetTypeFilters({});
	#endif
	
	ui.tabs.push_back(ImageTab());
	ui.initEffects(0);
	ui.io = io;

}

void VisualIDK::Update() {
	if (!ui.tabs[ui.current_tab].loading) {
		openFile();
		saveFile();
		//If User Pressed CTRL C
		copyMethod();
		pasteMethod();
	}
		
	//Zooms in and out of the image.
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		ui.tabs[ui.current_tab].zoom += io->MouseWheel/10.f;

	selectTool();
	ui.io = io;
	ui.Update();

}

void VisualIDK::imageRefreshing() {
	//Could be optimized.
	if (ui.tabs[ui.current_tab].loading)
		return;
	if ((Image::isEmpty(ui.tabs[ui.current_tab].original)) && noModif)
		return;
	bool effectsChanged = !noModif;
	for (size_t i = 0; !effectsChanged && i < ui.tabs[ui.current_tab].effects.size(); ++i)
		effectsChanged = ui.tabs[ui.current_tab].effects[i].active != ui.tabs[ui.current_tab].effectsCopy[i].active;
	if (!effectsChanged)
		return;
	
	applyEffects();
	ui.tabs[ui.current_tab].width = ui.tabs[ui.current_tab].post.getWidth();
	ui.tabs[ui.current_tab].height = ui.tabs[ui.current_tab].post.getHeight();
	
	if (ui.tabs[ui.current_tab].width != 0 && ui.tabs[ui.current_tab].height != 0) {
		ui.tabs[ui.current_tab].imageData.clear();
		for (int y = 0; y < ui.tabs[ui.current_tab].height; ++y) {
			for (int x = 0; x < ui.tabs[ui.current_tab].width; ++x) {
				ui.tabs[ui.current_tab].imageData.push_back(static_cast<unsigned char>(ui.tabs[ui.current_tab].post.img.r[x][y]));
				ui.tabs[ui.current_tab].imageData.push_back(static_cast<unsigned char>(ui.tabs[ui.current_tab].post.img.v[x][y]));
				ui.tabs[ui.current_tab].imageData.push_back(static_cast<unsigned char>(ui.tabs[ui.current_tab].post.img.b[x][y]));
				ui.tabs[ui.current_tab].imageData.push_back(static_cast<unsigned char>(ui.tabs[ui.current_tab].post.img.a[x][y]));
			}
		}
	}
	ui.tabs[ui.current_tab].effectsCopy = ui.tabs[ui.current_tab].effects;
	noModif = true;
}

void VisualIDK::Draw() {
	#ifdef USE_DUMB_DRAW
	for (uint32_t y = 0; y < post.getHeight(); ++y) {
		for (uint32_t x = 0; x < post.getWidth(); ++x) {
			dl->AddRectFilled({x*pixelSize,y*pixelSize}, {x*pixelSize+pixelSize, y*pixelSize+pixelSize},
			IM_COL32(post.img.r[x][y], post.img.v[x][y], post.img.b[x][y], 255));
		}
	}
	#else
	
	//ImGui::SetCursorPos(ImVec2(10.0f, 10.0f));  // Set the position where you want to render the image
	//ImGui::Image((void*)(intptr_t)textureID, ImVec2(post.img.r.size(), post.img.r[0].size()));
	ImGui::Begin("Image", (bool*)__null, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
	if (!ui.tabs[ui.current_tab].loading) {
		if (!ui.hasSetDefaultSizes) {
			ImGui::SetWindowPos({ui.ImageSize.x, ui.ImageSize.y});
			ImGui::SetWindowSize({ui.ImageSize.z, ui.ImageSize.w});
		}
		
		ImGui::SetWindowSize(ImVec2(io->DisplaySize.x - ui.EditingSize.z - ui.ToolbarSize.z, io->DisplaySize.y-30-18-32));
		ImGui::SetWindowPos({ui.ToolbarSize.z, ui.MenuBarSize.w});
		ui.ImageSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		if (ui.tabs[ui.current_tab].width != 0 && ui.tabs[ui.current_tab].height != 0) {
			glGenTextures(1, &ui.tabs[ui.current_tab].textureID);
			glBindTexture(GL_TEXTURE_2D, ui.tabs[ui.current_tab].textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ui.tabs[ui.current_tab].post.img.r.size(), ui.tabs[ui.current_tab].post.img.r[0].size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, ui.tabs[ui.current_tab].imageData.data());

			// Set texture parameters (you may need to adjust these based on your requirements)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			// Use the shader program, bind the texture, and draw a quad
			glBindTexture(GL_TEXTURE_2D, ui.tabs[ui.current_tab].textureID);
			ImGui::Image(
				(void*)(intptr_t)ui.tabs[ui.current_tab].textureID,
				ImVec2(ui.tabs[ui.current_tab].width * ui.tabs[ui.current_tab].zoom, ui.tabs[ui.current_tab].height * ui.tabs[ui.current_tab].zoom)
			);
		}
	} else {
		ImGui::TextColored(
			ui.darkMode ? ImVec4(255,255,255,255) : ImVec4(0,0,0,255), 
			"Loading image..."
		);
	}
	
	#endif //USE_DUMB_DRAW
	ImGui::End();
	#ifdef __linux__
	fileDialog.Display();
	fileDialogSave.Display();
	#endif
	
}

void VisualIDK::copyMethod() {
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_C) && !Image::isEmpty(ui.tabs[ui.current_tab].post)) {
		#ifdef __linux__
		clip::image_spec spec;
		spec.width = ui.tabs[ui.current_tab].width;
		spec.height = ui.tabs[ui.current_tab].height;
		spec.bits_per_pixel = 32;
		spec.bytes_per_row = spec.width*4;
		spec.red_mask = 0xff;
		spec.green_mask = 0xff00;
		spec.blue_mask = 0xff0000;
		spec.alpha_mask = 0xff000000;
		spec.red_shift = 0;
		spec.green_shift = 8;
		spec.blue_shift = 16;
		spec.alpha_shift = 24;
		clip::image img(ui.tabs[ui.current_tab].imageData.data(), spec);
		clip::set_image(img);
		#endif
	}
}

void VisualIDK::pasteMethod() {
	if (!(ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_V)))
		return;
	#ifdef __linux__
	if (!clip::has(clip::image_format())) {
		std::cout << "Clipboard doesn't contain an image\n";
	} else {
		clip::image img;
		if (!clip::get_image(img)) {
			std::cout << "Error getting image from clipboard\n";
		} else {
			clip::image_spec spec = img.spec();
			ui.tabs[ui.current_tab].original = Image(spec.width, spec.height);
			auto data2 = (const uint32_t*)img.data();
			for (uint32_t iy = 0; iy < spec.height; ++iy) {
				for (uint32_t ix = 0; ix < spec.width; ix++) {
					// Assign values to the corresponding arrays
					ui.tabs[ui.current_tab].original.img.r[ix][iy] = data2[iy * spec.width + ix];
					ui.tabs[ui.current_tab].original.img.v[ix][iy] = data2[iy * spec.width + ix] >> 8;
					ui.tabs[ui.current_tab].original.img.b[ix][iy] = data2[iy * spec.width + ix] >> 16;
				}
			}
			ui.tabs[ui.current_tab].post = ui.tabs[ui.current_tab].original;
			noModif = false;
		}
	}
	#elif _WIN32
	if (IsClipboardFormatAvailable(CF_DIB)) {
		if (OpenClipboard(NULL)) {
			HANDLE hClipboard = GetClipboardData(CF_DIB);
			if (hClipboard != NULL && hClipboard != INVALID_HANDLE_VALUE) {
				void* dib = GlobalLock(hClipboard);

				if (dib) {
					BITMAPINFOHEADER* info = reinterpret_cast<BITMAPINFOHEADER*>(dib);
					BITMAPFILEHEADER fileHeader = {0};
					fileHeader.bfType = 0x4D42;
					fileHeader.bfOffBits = 54;

					std::cout << "Type: " << std::hex << fileHeader.bfType << std::dec << "\n";
					std::cout << "Reserved: " << fileHeader.bfReserved1 << "\n";
					std::cout << "Reserved2: " << fileHeader.bfReserved2 << "\n";
					std::cout << "Offset: " << fileHeader.bfOffBits << "\n";
					std::cout << "Width: " << info->biWidth << "\n";
					std::cout << "Height: " << info->biHeight << "\n";

					// Calculate the size of the image data
					DWORD imageSize = info->biSizeImage;
					if (imageSize == 0) {
						// If the size is not specified, calculate it based on width, height, and bit depth
						imageSize = info->biWidth * info->biHeight * (info->biBitCount / 8);
					}

					// Get a pointer to the start of the pixel data
					uint8_t* imageData = reinterpret_cast<uint8_t*>(dib);//+ fileHeader.bfOffBits;

					// Now you can access the pixel data in the 'imageData' buffer

					// For example, print the first few bytes of the pixel data
					ui.tabs[ui.current_tab].original = Image(info->biWidth, info->biHeight);
					ui.tabs[ui.current_tab].width = info->biWidth;
					ui.tabs[ui.current_tab].height = info->biHeight;
					for (uint32_t iy = 0; iy < static_cast<uint32_t>(info->biHeight); ++iy) {
						for (uint32_t ix = 0; ix < static_cast<uint32_t>(info->biWidth); ++ix) {
							// Calculate the offset in the imageData buffer for the current pixel
							size_t offset = (static_cast<uint32_t>(info->biHeight) - static_cast<size_t>(iy) - 1) * (static_cast<size_t>(info->biWidth) * 4) + static_cast<size_t>(ix) * 4 + fileHeader.bfOffBits+5;
							// Extract the BGR values
							uint8_t blue = imageData[offset + 1];
							uint8_t green = imageData[offset + 2];
							uint8_t red = imageData[offset + 3];

							// Assign the values to the image arrays
							ui.tabs[ui.current_tab].original.img.r[ix][iy] = red;
							ui.tabs[ui.current_tab].original.img.v[ix][iy] = green;
							ui.tabs[ui.current_tab].original.img.b[ix][iy] = blue;
						}
					}

					GlobalUnlock(dib);
				}
			}

			CloseClipboard();
		}
	}
	#endif
	//ui.tabs[current_tab].title = "image.impaint";
}

void VisualIDK::openFile() {
	if (ui.mainMenu[MENU_FILE].buttons[MENU_FILE_OPEN].active || (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_O))) {
		#ifdef _WIN32
		#ifndef IDE_ANTI_ERROR
		OPENFILENAME ofn ;
		char szFile[MAX_PATH] = {0};
		ZeroMemory( &ofn , sizeof( ofn));
		ofn.lStructSize = sizeof ( ofn );
		ofn.hwndOwner = NULL  ;
		ofn.lpstrFile = szFile ;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof( szFile );
		ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex =1;
		ofn.lpstrFileTitle = NULL ;
		ofn.nMaxFileTitle = 0 ;
		ofn.lpstrInitialDir=NULL ;
		ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
		if (GetOpenFileName(&ofn)) {
			//MessageBox ( NULL , ofn.lpstrFile , "test" , MB_OK);
			ui.tabs[ui.current_tab].loading = true;
			
			fileLoadingThread = thread(&VisualIDK::loadFile, this, string(ofn.lpstrFile), ui.current_tab);
			//loadFile(string(ofn.lpstrFile), current_tab);
		}
		#endif
		#elif __linux__
			fileDialog.Open();

		#endif
		ui.mainMenu[MENU_FILE].buttons[MENU_FILE_OPEN].active = false;
	}

	#ifdef __linux__
	if(fileDialog.HasSelected())
	{
		ui.tabs[ui.current_tab].original = Image(fileDialog.GetSelected().string());
		ui.tabs[ui.current_tab].post = ui.tabs[ui.current_tab].original;
		noModif = false;
		fileDialog.ClearSelected();
	}
	#endif
}

void VisualIDK::saveFile() {
	if ((ui.mainMenu[MENU_FILE].active && ui.mainMenu[MENU_FILE].buttons[MENU_FILE_SAVE].active) || (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S))) {
		cout << ((ui.mainMenu[MENU_FILE].active && ui.mainMenu[MENU_FILE].buttons[MENU_FILE_SAVE].active) ? "true" : "false") << endl;
	#ifdef __linux__
		fileDialogSave.Open();
	}

	if(fileDialogSave.HasSelected())
	{
		ui.tabs[ui.current_tab].post.write(fileDialogSave.GetSelected().string());
		fileDialogSave.ClearSelected();
		ui.mainMenu[MENU_FILE].buttons[MENU_FILE_SAVE].active = false;
	}
	#elif _WIN32
		#ifndef IDE_ANTI_ERROR
		OPENFILENAME ofn ;
		char szFile[MAX_PATH] = {0};
		ZeroMemory( &ofn , sizeof( ofn));
		ofn.lStructSize = sizeof ( ofn );
		ofn.hwndOwner = NULL  ;
		ofn.lpstrFile = szFile ;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof( szFile );
		ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex =1;
		ofn.lpstrFileTitle = NULL ;
		ofn.nMaxFileTitle = 0 ;
		ofn.lpstrInitialDir=NULL ;
		ofn.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST ;
		if (GetSaveFileName(&ofn))
			ui.tabs[ui.current_tab].post.write(ofn.lpstrFile);
		#endif
	#endif
	}
}

void VisualIDK::applyEffects() {
	ui.tabs[ui.current_tab].post = ui.tabs[ui.current_tab].original;
	for (size_t i = 0; i < ui.tabs[ui.current_tab].effects.size(); ++i) {
		if (ui.tabs[ui.current_tab].effects[i].active) {
			switch (ui.tabs[ui.current_tab].effects[i].which)
			{
			case 0:
				ui.tabs[ui.current_tab].post = (ui.tabs[ui.current_tab].post.*(ui.tabs[ui.current_tab].effects[i].func))();
				break;
			
			case 1:
				ui.tabs[ui.current_tab].post = (ui.tabs[ui.current_tab].post.*(ui.tabs[ui.current_tab].effects[i].funcFloat))(ui.tabs[ui.current_tab].effects[i].argFloat);
				break;

			case 2:
				ui.tabs[ui.current_tab].post = (ui.tabs[ui.current_tab].post.*(ui.tabs[ui.current_tab].effects[i].funcUINT32T))(ui.tabs[ui.current_tab].effects[i].argUint32);
				break;

			case 3:
				ui.tabs[ui.current_tab].post = Filter(ui.tabs[ui.current_tab].effects[i].argFilter).application(ui.tabs[ui.current_tab].post);
				break;
			
			default:
				break;
			}
		}
	}
}

void VisualIDK::selectTool() {
	
}

VisualIDK::~VisualIDK() {
	if (fileLoadingThread.joinable())
		fileLoadingThread.join();
}