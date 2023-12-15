#include "visual.h"

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
		{false, 3, nullptr, nullptr, nullptr, CONTRASTOR},
		{false, 0, &Image::reglageAuto},
		{false, 0, &Image::reglageAutoGris},
		{false, 0, &Image::reglageAutoCouleur},
	};
}

VisualIDK::VisualIDK() {
	mainMenu.push_back((MainMenuDropdown){"File", false, vector<MainMenuDropdownButton>{{"New", false}, {"Load", false}, {"Save", false}}});
	mainMenu.push_back((MainMenuDropdown){"Edit", false, vector<MainMenuDropdownButton>{{"Cut", false}, {"Copy", false}, {"Paste", false}}});
	mainMenu.push_back((MainMenuDropdown){"Display", false, vector<MainMenuDropdownButton>{{"NBGC", false}, {"Géométrie", false}, {"Filtres", false}, {"Toggle dark mode", false}}});

	fileDialog.SetTitle("Open...");
	fileDialog.SetTypeFilters({".ppm"});
	
	fileDialogSave.SetTitle("Save...");
	fileDialogSave.SetTypeFilters({".ppm"});
	mainMenu[2].buttons[0].active = true;
	mainMenu[2].buttons[1].active = true;
	mainMenu[2].buttons[2].active = true;

	MenuBarSize = {0, 0, -1, (18+30)};
	ImageSize = {64, (18+30), 1280-240-64, 720-(18+30)};
	EditingSize = {1280 - 240, (18+30), 240, 720 - (18+30)};
	ToolbarSize = {0, (18+30), 64, 720 - (18+30)};

	tabs.push_back(ImageTab());
	tabs[0].initEffects();
}

void VisualIDK::Update() {
	if (!noModif) {
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
	}
	
	if (mainMenu[0].buttons[1].active || (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_O))) {
		#ifdef _WIN32
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
		GetOpenFileName(&ofn);
		//MessageBox ( NULL , ofn.lpstrFile , "test" , MB_OK);

		tabs[current_tab].original = Image(ofn.lpstrFile);
		tabs[current_tab].post = tabs[current_tab].original;
		#elif __linux__
			fileDialog.Open();

		#endif
		mainMenu[0].buttons[1].active = false;
	}
	#ifdef __linux__
	if ((mainMenu[0].active && mainMenu[0].buttons[2].active) || (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_S))) {
		fileDialogSave.Open();
	}

	if(fileDialog.HasSelected())
	{
		tabs[current_tab].original = Image(fileDialog.GetSelected().string());
		tabs[current_tab].post = tabs[current_tab].original;
		noModif = false;
		fileDialog.ClearSelected();
	}

	if(fileDialogSave.HasSelected())
	{
		tabs[current_tab].post.write(fileDialogSave.GetSelected().string());
		fileDialogSave.ClearSelected();
		mainMenu[0].buttons[2].active = false;
	}
	#endif
	if (mainMenu[2].buttons[3].active) {
		ImGui::StyleColorsDark();
		clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

	} else {
		ImGui::StyleColorsLight();
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}
	
	if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl))
		tabs[current_tab].zoom += io->MouseWheel/10.f;

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

	// Interleave RGB values
	if (!noModif) {
		tabs[current_tab].width = tabs[current_tab].post.getWidth();
		tabs[current_tab].height = tabs[current_tab].post.getHeight();
		noModif = true;
		if (tabs[current_tab].width != 0 && tabs[current_tab].height != 0) {
			tabs[current_tab].imageData.clear();
			for (int y = 0; y < tabs[current_tab].height; ++y) {
				for (int x = 0; x < tabs[current_tab].width; ++x) {
					tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.r[x][y]));
					tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.v[x][y]));
					tabs[current_tab].imageData.push_back(static_cast<unsigned char>(tabs[current_tab].post.img.b[x][y]));
					tabs[current_tab].imageData.push_back(static_cast<unsigned char>(255));
				}
			}

			glGenTextures(1, &tabs[current_tab].textureID);
			glBindTexture(GL_TEXTURE_2D, tabs[current_tab].textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tabs[current_tab].post.img.r.size(), tabs[current_tab].post.img.r[0].size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tabs[current_tab].imageData.data());

			// Set texture parameters (you may need to adjust these based on your requirements)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			// Use the shader program, bind the texture, and draw a quad
			glBindTexture(GL_TEXTURE_2D, tabs[current_tab].textureID);
		}
	
	}
	//ImGui::SetCursorPos(ImVec2(10.0f, 10.0f));  // Set the position where you want to render the image
	//ImGui::Image((void*)(intptr_t)textureID, ImVec2(post.img.r.size(), post.img.r[0].size()));
	ImGui::Begin("Image", (bool*)__null, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoResize);
	if (!hasSetDefaultSizes) {
		ImGui::SetWindowPos({ImageSize.x, ImageSize.y});
		ImGui::SetWindowSize({ImageSize.z, ImageSize.w});
	}
	ImGui::SetWindowSize(ImVec2(io->DisplaySize.x - EditingSize.z - ToolbarSize.z, io->DisplaySize.y));
	ImGui::SetWindowPos({ToolbarSize.z, MenuBarSize.w});
	ImageSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	if (tabs[current_tab].width != 0 && tabs[current_tab].height != 0)
		ImGui::Image(
			(void*)(intptr_t)tabs[current_tab].textureID,
			ImVec2(tabs[current_tab].width * tabs[current_tab].zoom, tabs[current_tab].height * tabs[current_tab].zoom)
		);
	#endif //USE_DUMB_DRAW
	ImGui::End();
	fileDialog.Display();
	fileDialogSave.Display();
}

void VisualIDK::UI() {
	ImGui::Begin("debtabs", (bool*)__null, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::BeginTabBar("Tabsa", ImGuiTabBarFlags_NoTooltip);
	ImGui::SetWindowPos({0, 18});
	ImGui::SetWindowSize({io->DisplaySize.x, 30});
	
	for (size_t i = 0; i < tabs.size(); ++i) {
		if (ImGui::TabItemButton(to_string(i).c_str()))
			current_tab = i;
	}
	
	if(ImGui::TabItemButton("+")) {
		tabs.push_back(ImageTab());
		tabs[tabs.size()-1].initEffects();
	}
	ImGui::EndTabBar();
	ImGui::End();
	UIToolbar();
	UIEditing();
	UIMenuBar();

	

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
}

void VisualIDK::UIEditing() {
	if (mainMenu[2].buttons[0].active || mainMenu[2].buttons[1].active) {
		ImGui::Begin("Editing");
		if (!hasSetDefaultSizes) {
			ImGui::SetWindowPos({EditingSize.x, EditingSize.y});
			ImGui::SetWindowSize({EditingSize.z, EditingSize.w});
		}
		EditingSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
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
			ImGui::Checkbox("reglageAuto", &tabs[current_tab].effects[EFFECTS_reglageAuto].active);
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
	ImGui::Text("txt");
	ImGui::End();
	
}