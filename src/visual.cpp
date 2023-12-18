#include "visual.h"
void VisualIDK::loadFile(const string& filePath, size_t loadingTab) {
	if (!tabs[loadingTab].loading)
		return;
	tabs[loadingTab].original = Image(filePath);
	tabs[loadingTab].post = tabs[loadingTab].original;
	tabs[loadingTab].loading = false;
}

void ImageTab::initEffects() {
	effects = {
		{false, 0, &Image::blackWhite},
		{false, 0, &Image::redCanal},
		{false, 0, &Image::grayScale},
		{false, 1, nullptr, &Image::changeLuminosity},
		{false, 1, nullptr, &Image::changeContraste},
		{false, 0, &Image::rotationR},
		{false, 0, &Image::rotationL},
		{false, 0, &Image::spinH},
		{false, 0, &Image::spinV},
		{false, 2, nullptr, nullptr, &Image::clipR},
		{false, 2, nullptr, nullptr, &Image::clipL},
		{false, 2, nullptr, nullptr, &Image::clipU},
		{false, 2, nullptr, nullptr, &Image::clipD},
		{false, 2, nullptr, nullptr, &Image::enlarge},
		{false, 2, nullptr, nullptr, &Image::shrink},
		{false, 0, &Image::colorblindDeuteranopia},
		{false, 0, &Image::colorblindProtanopia},
		{false, 0, &Image::colorblindTritanopia},
		{false, 3, nullptr, nullptr, nullptr, BLURG3},
		{false, 3, nullptr, nullptr, nullptr, BLURG5},
		{false, 0, &Image::sobelOperator},
		{false, 3, nullptr, nullptr, nullptr, CONTRASTOR}
	};
}

VisualIDK::VisualIDK() {
	mainMenu.push_back((MainMenuDropdown){"File", false, vector<MainMenuDropdownButton>{{"New", false}, {"Load", false}, {"Save", false}}});
	mainMenu.push_back((MainMenuDropdown){"Edit", false, vector<MainMenuDropdownButton>{{"Cut", false}, {"Copy", false}, {"Paste", false}}});
	mainMenu.push_back((MainMenuDropdown){"Display", false, vector<MainMenuDropdownButton>{{"NBGC", false}, {"Géométrie", false}, {"Filtres", false}, {"Toggle dark mode", false}}});
	#ifdef __linux__
	fileDialog.SetTitle("Open...");
	fileDialog.SetTypeFilters({".ppm", ".png", ".bmp", ".jpg", ".jpeg"});
	
	fileDialogSave.SetTitle("Save...");
	fileDialogSave.SetTypeFilters({});
	#endif
	mainMenu[2].buttons[0].active = true;
	mainMenu[2].buttons[1].active = true;
	mainMenu[2].buttons[2].active = true;

	MenuBarSize = {0, 0, -1, (18+30)};
	ImageSize = {64, (18+30), 1280-240-64, 720-(18+30)-16};
	EditingSize = {1280 - 240, (18+30), 240, 720 - (18+30)};
	ToolbarSize = {0, (18+30), 64, 720 - (18+30)};

	tabs.push_back(ImageTab());
	tabs[0].initEffects();
}

void VisualIDK::Update() {
	//applyEffects();
	if (!tabs[current_tab].loading) {
		openFile();
		saveFile();
		//If User Pressed CTRL C
		copyMethod();
		pasteMethod();
	}
	

	//Toggle Dark Mode
	if (mainMenu[2].buttons[3].active) {
		ImGui::StyleColorsDark();
		clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

	} else {
		ImGui::StyleColorsLight();
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}
	
	//Zooms in and out of the image.
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		tabs[current_tab].zoom += io->MouseWheel/10.f;

	selectTool();

}

void VisualIDK::imageRefreshing() {
	if (tabs[current_tab].loading || Image::isEmpty(tabs[current_tab].original))
		return;
	applyEffects();
	tabs[current_tab].width = tabs[current_tab].post.getWidth();
	tabs[current_tab].height = tabs[current_tab].post.getHeight();
	if (tabs[current_tab].width != 0 && tabs[current_tab].height != 0) {
		tabs[current_tab].imageData.clear();
		for (int y = 0; y < tabs[current_tab].height; ++y) {
			for (int x = 0; x < tabs[current_tab].width; ++x) {
				tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.r[x][y]));
				tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.v[x][y]));
				tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.b[x][y]));
				tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.a[x][y]));
			}
		}
	}
}

void VisualIDK::Draw(thread &func) {
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
	if (!tabs[current_tab].loading) {
		if (!hasSetDefaultSizes) {
			ImGui::SetWindowPos({ImageSize.x, ImageSize.y});
			ImGui::SetWindowSize({ImageSize.z, ImageSize.w});
		}
		ImGui::SetWindowSize(ImVec2(io->DisplaySize.x - EditingSize.z - ToolbarSize.z, io->DisplaySize.y-30-18-32));
		ImGui::SetWindowPos({ToolbarSize.z, MenuBarSize.w});
		ImageSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		if (tabs[current_tab].width != 0 && tabs[current_tab].height != 0) {
			glGenTextures(1, &tabs[current_tab].textureID);
			glBindTexture(GL_TEXTURE_2D, tabs[current_tab].textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tabs[current_tab].post.img.r.size(), tabs[current_tab].post.img.r[0].size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tabs[current_tab].imageData.data());

			// Set texture parameters (you may need to adjust these based on your requirements)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			
			// Use the shader program, bind the texture, and draw a quad
			glBindTexture(GL_TEXTURE_2D, tabs[current_tab].textureID);
			ImGui::Image(
				(void*)(intptr_t)tabs[current_tab].textureID,
				ImVec2(tabs[current_tab].width * tabs[current_tab].zoom, tabs[current_tab].height * tabs[current_tab].zoom)
			);
		}
	} else {
		ImGui::TextColored(
			mainMenu[2].buttons[3].active ? ImVec4(255,255,255,255) : ImVec4(0,0,0,255), 
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

void VisualIDK::UI() {
	UITabbar();
	UIToolbar();
	UIEditing();
	UIMenuBar();
	UIErrorBay();

	hasSetDefaultSizes = true;
}

void VisualIDK::UIMenuBar() {
	// Start the menu bar
	if (ImGui::BeginMainMenuBar()) {
		
		for (size_t i = 0; i < mainMenu.size(); ++i) {
			mainMenu[i].active = ImGui::BeginMenu(mainMenu[i].title.c_str());
			if (mainMenu[i].active) {
				for (size_t j = 0; j < mainMenu[i].buttons.size(); ++j)
					if (ImGui::MenuItem(mainMenu[i].buttons[j].title.c_str()))
						mainMenu[i].buttons[j].active = !mainMenu[i].buttons[j].active;
				ImGui::EndMenu(); 
			}
		}
		
		ImGui::EndMainMenuBar();
	}

	//If User pressed CCTRL+N or File->New.
	if (mainMenu[0].buttons[0].active || (ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N))) {
		tabs.push_back(ImageTab());
		tabs[tabs.size()-1].initEffects();
		mainMenu[0].buttons[0].active = false;
	}
}

void VisualIDK::UIEditing() {
	if (mainMenu[2].buttons[0].active || mainMenu[2].buttons[1].active) {
		ImGui::Begin("Editing");
		if (!hasSetDefaultSizes) {
			ImGui::SetWindowPos({EditingSize.x, EditingSize.y});
			ImGui::SetWindowSize({EditingSize.z, EditingSize.w});
		}
		EditingSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		ImGui::SetWindowPos({io->DisplaySize.x-EditingSize.z, EditingSize.y});
		ImGui::SetWindowSize({ ImGui::GetWindowSize().x, io->DisplaySize.y - EditingSize.y});
		if (ImGui::Button("Refresh")) noModif = false;
		ImGui::SameLine(0.f);
		ImGui::Text("FPS: %1.f", 1.f/io->DeltaTime);
		if (mainMenu[2].buttons[0].active) {
			ImGui::Text("Effects");
			ImGui::Checkbox("Black & WHite", &tabs[current_tab].effects[EFFECTS_blackWhite].active);
			ImGui::Checkbox("Red Canal", &tabs[current_tab].effects[EFFECTS_redChannel].active);
			ImGui::Checkbox("Gray Scale", &tabs[current_tab].effects[EFFECTS_grayScale].active);
			ImGui::Checkbox("Luminosity", &tabs[current_tab].effects[EFFECTS_luminosity].active);
			ImGui::SameLine(0.f);
			ImGui::PushItemWidth(96);
			ImGui::InputFloat("l", &tabs[current_tab].effects[EFFECTS_luminosity].argFloat, 0.1f, 0.2f);
			ImGui::PopItemWidth();
			ImGui::Checkbox("Contrast", &tabs[current_tab].effects[EFFECTS_contrast].active);
			ImGui::SameLine(0.f);
			ImGui::PushItemWidth(96);
			ImGui::InputFloat("c", &tabs[current_tab].effects[EFFECTS_contrast].argFloat, 0.1f, 0.2f);
			ImGui::PopItemWidth();
			ImGui::Text("Colorblindness:");
			ImGui::SameLine(0.f);
			ImGui::Checkbox("T", &tabs[current_tab].effects[EFFECTS_colorblindTritanopia].active);
			ImGui::SameLine(0.f);
			ImGui::Checkbox("P", &tabs[current_tab].effects[EFFECTS_colorblindProtanopia].active);
			ImGui::SameLine(0.f);
			ImGui::Checkbox("De", &tabs[current_tab].effects[EFFECTS_colorblindDeuteranopia].active);
			ImGui::NewLine();
		}
		if (mainMenu[2].buttons[1].active) {
			ImGui::Text("Geometry");
			ImGui::Checkbox("Rotation Right", &tabs[current_tab].effects[EFFECTS_rotationR].active);
			ImGui::Checkbox("Rotation Left", &tabs[current_tab].effects[EFFECTS_rotationL].active);
			ImGui::Text("Return");
			ImGui::SameLine(0.f);
			ImGui::Checkbox("Ho", &tabs[current_tab].effects[EFFECTS_spinH].active);
			ImGui::SameLine(0.f);
			ImGui::Checkbox("Ve", &tabs[current_tab].effects[EFFECTS_spinV].active);
			ImGui::Text("Clip");
			ImGui::SameLine(0.f);
			ImGui::Checkbox("R", &tabs[current_tab].effects[EFFECTS_clipR].active);
			ImGui::SameLine(0.f);
			ImGui::Checkbox("L", &tabs[current_tab].effects[EFFECTS_clipL].active);
			ImGui::SameLine(0.f);
			ImGui::Checkbox("U", &tabs[current_tab].effects[EFFECTS_clipU].active);
			ImGui::SameLine(0.f);
			ImGui::Checkbox("D", &tabs[current_tab].effects[EFFECTS_clipD].active);
			ImGui::Checkbox("Enlarge", &tabs[current_tab].effects[EFFECTS_enlarge].active);
			ImGui::SameLine(0.f);
			ImGui::PushItemWidth(64);
			ImGui::InputInt("aV", (int*)&tabs[current_tab].effects[EFFECTS_enlarge].argUint32, 1, 2);
			ImGui::PopItemWidth();
			ImGui::Checkbox("Shrink", &tabs[current_tab].effects[EFFECTS_shrink].active);
			ImGui::SameLine(0.f);
			ImGui::PushItemWidth(64);
			ImGui::InputInt("rV", (int*)&tabs[current_tab].effects[EFFECTS_luminosity].argUint32, 1, 2);
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		if (mainMenu[2].buttons[2].active) {
			ImGui::Text("Filters");
			ImGui::Checkbox("BlurG3", &tabs[current_tab].effects[EFFECTS_filterBlurG3].active);
			ImGui::Checkbox("BlurG5", &tabs[current_tab].effects[EFFECTS_filterBlurG5].active);
			ImGui::Checkbox("Sobel Operator", &tabs[current_tab].effects[EFFECTS_filterSoberOperator].active);
			ImGui::Checkbox("Constrastor", &tabs[current_tab].effects[EFFECTS_filtreContrastor].active);
			ImGui::NewLine();
		}
		
		ImGui::End();
		
	}
}

void VisualIDK::UIToolbar() {
	ImGui::Begin("Toolbar");
	if (!hasSetDefaultSizes) {
		ImGui::SetWindowPos(ImVec2(ToolbarSize.x,ToolbarSize.y));
		ImGui::SetWindowSize(ImVec2(ToolbarSize.z, ToolbarSize.w));
	}
	ToolbarSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	ImGui::SetWindowPos(ImVec2(ToolbarSize.x,ToolbarSize.y));
	ImGui::SetWindowSize(ImVec2(ToolbarSize.z, io->DisplaySize.y-ToolbarSize.y));
	ImGui::Text("txt");
	ImGui::End();
	
}

void VisualIDK::UIErrorBay() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.f, 0.f));
	ImGui::Begin("ErrorTab", (bool*)__null, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("%s", tabs[current_tab].post.getError().c_str() );
	ImGui::SetWindowPos({ToolbarSize.z, io->DisplaySize.y-32});
	ImGui::SetWindowSize(ImVec2(io->DisplaySize.x - EditingSize.z - ToolbarSize.z, 32));
	ImGui::End();
	ImGui::PopStyleVar();
}

void VisualIDK::UITabbar() {
	ImGui::Begin("debtabs", (bool*)__null, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::BeginTabBar("Tabsa", ImGuiTabBarFlags_NoTooltip);
	ImGui::SetWindowPos({0, 18});
	ImGui::SetWindowSize({io->DisplaySize.x, 30});
	
	for (size_t i = 0; i < tabs.size(); ++i) {
		if (current_tab == i) 
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, (mainMenu[2].buttons[3].active) ? 1 : 0, 1, 1));
		bool hasClicked = ImGui::TabItemButton(to_string(i).c_str());
		if (hasClicked)	
			current_tab = i;
		if (current_tab == i && !hasClicked)
			ImGui::PopStyleColor();
	}
	
	if(ImGui::TabItemButton("+")) {
		tabs.push_back(ImageTab());
		tabs[tabs.size()-1].initEffects();
	}
	ImGui::EndTabBar();
	ImGui::End();
}

void VisualIDK::copyMethod() {
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_C) && !Image::isEmpty(tabs[current_tab].post)) {
		#ifdef __linux__
		clip::image_spec spec;
		spec.width = tabs[current_tab].width;
		spec.height = tabs[current_tab].height;
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
		clip::image img(tabs[current_tab].imageData.data(), spec);
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
			tabs[current_tab].original = Image(spec.width, spec.height);
			auto data2 = (const uint32_t*)img.data();
			for (uint32_t iy = 0; iy < spec.height; ++iy) {
				for (uint32_t ix = 0; ix < spec.width; ix++) {
					// Assign values to the corresponding arrays
					tabs[current_tab].original.img.r[ix][iy] = data2[iy * spec.width + ix];
					tabs[current_tab].original.img.v[ix][iy] = data2[iy * spec.width + ix] >> 8;
					tabs[current_tab].original.img.b[ix][iy] = data2[iy * spec.width + ix] >> 16;
				}
			}
			tabs[current_tab].post = tabs[current_tab].original;
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
					tabs[current_tab].original = Image(info->biWidth, info->biHeight);
					for (uint32_t iy = 0; iy < static_cast<uint32_t>(info->biHeight); ++iy) {
						for (uint32_t ix = 0; ix < static_cast<uint32_t>(info->biWidth); ++ix) {
							// Calculate the offset in the imageData buffer for the current pixel
							size_t offset = (static_cast<uint32_t>(info->biHeight) - static_cast<size_t>(iy) - 1) * (static_cast<size_t>(info->biWidth) * 4) + static_cast<size_t>(ix) * 4 + fileHeader.bfOffBits+5;
							// Extract the BGR values
							uint8_t blue = imageData[offset + 1];
							uint8_t green = imageData[offset + 2];
							uint8_t red = imageData[offset + 3];

							// Assign the values to the image arrays
							tabs[current_tab].original.img.r[ix][iy] = red;
							tabs[current_tab].original.img.v[ix][iy] = green;
							tabs[current_tab].original.img.b[ix][iy] = blue;
						}
					}

					GlobalUnlock(dib);
				}
			}

			CloseClipboard();
		}
	}
	#endif
}

void VisualIDK::openFile() {
	if (mainMenu[0].buttons[1].active || (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_O))) {
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
			tabs[current_tab].loading = true;
			if (fileLoadingThread.joinable())
				fileLoadingThread.join();
			fileLoadingThread = thread(&VisualIDK::loadFile, this, string(ofn.lpstrFile), current_tab);

		}
		#endif
		#elif __linux__
			fileDialog.Open();

		#endif
		mainMenu[0].buttons[1].active = false;
	}

	#ifdef __linux__
	if(fileDialog.HasSelected())
	{
		tabs[current_tab].original = Image(fileDialog.GetSelected().string());
		tabs[current_tab].post = tabs[current_tab].original;
		noModif = false;
		fileDialog.ClearSelected();
	}
	#endif
}

void VisualIDK::saveFile() {
	if ((mainMenu[0].active && mainMenu[0].buttons[2].active) || (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S))) {
		cout << ((mainMenu[0].active && mainMenu[0].buttons[2].active) ? "true" : "false") << endl;
	#ifdef __linux__
		fileDialogSave.Open();
	}

	if(fileDialogSave.HasSelected())
	{
		tabs[current_tab].post.write(fileDialogSave.GetSelected().string());
		fileDialogSave.ClearSelected();
		mainMenu[0].buttons[2].active = false;
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
			tabs[current_tab].post.write(ofn.lpstrFile);
		#endif
	#endif
	}
}

void VisualIDK::applyEffects() {
	//if (!noModif) {
		tabs[current_tab].post = tabs[current_tab].original;
		for (size_t i = 0; i < tabs[current_tab].effects.size(); ++i) {
			if (tabs[current_tab].effects[i].active) {
				switch (tabs[current_tab].effects[i].which)
				{
				case 0:
					tabs[current_tab].post = (tabs[current_tab].post.*(tabs[current_tab].effects[i].func))();
					break;
				
				case 1:
					tabs[current_tab].post = (tabs[current_tab].post.*(tabs[current_tab].effects[i].funcFloat))(tabs[current_tab].effects[i].argFloat);
					break;

				case 2:
					tabs[current_tab].post = (tabs[current_tab].post.*(tabs[current_tab].effects[i].funcUINT32T))(tabs[current_tab].effects[i].argUint32);
					break;

				case 3:
					tabs[current_tab].post = Filter(tabs[current_tab].effects[i].argFilter).application(tabs[current_tab].post);
					break;
				
				default:
					break;
				}
			}
		}
	//}
}

void VisualIDK::selectTool() {
	
}