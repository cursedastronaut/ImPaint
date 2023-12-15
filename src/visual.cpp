#include "visual.h"

VisualIDK::VisualIDK() {
	post = original;
	mainMenu.push_back((MainMenuDropdown){"File", false, vector<MainMenuDropdownButton>{{"New", false}, {"Load", false}, {"Save", false}}});
	mainMenu.push_back((MainMenuDropdown){"Edit", false, vector<MainMenuDropdownButton>{{"Cut", false}, {"Copy", false}, {"Paste", false}}});
	mainMenu.push_back((MainMenuDropdown){"Display", false, vector<MainMenuDropdownButton>{{"NBGC", false}, {"Géométrie", false}, {"Filtres", false}, {"Toggle dark mode", false}}});

	fileDialog.SetTitle("title");
	fileDialog.SetTypeFilters({".ppm"});
	mainMenu[2].buttons[0].active = true;
	mainMenu[2].buttons[1].active = true;
	mainMenu[2].buttons[2].active = true;

	MenuBarSize = {0, 0, -1, 18};
	ImageSize = {64, 18, 1280-240-64, 720-18};
	EditingSize = {1280 - 240, 18, 240, 720 - 18};
	ToolbarSize = {0, 18, 64, 720 - 18};

	effects = {
		{false, 0, &Image::noirEtBlanc},
		{false, 0, &Image::composanteRouge},
		{false, 0, &Image::niveauxGris},
		{false, 1, nullptr, &Image::changeLuminosity},
		{false, 1, nullptr, &Image::changeContraste},
		{false, 0, &Image::rotationD},
		{false, 0, &Image::rotationG},
		{false, 0, &Image::retournementH},
		{false, 0, &Image::retournementV},
		{false, 2, nullptr, nullptr, &Image::rognerD},
		{false, 2, nullptr, nullptr, &Image::rognerG},
		{false, 2, nullptr, nullptr, &Image::rognerH},
		{false, 2, nullptr, nullptr, &Image::rognerB},
		{false, 2, nullptr, nullptr, &Image::agrandissement},
		{false, 2, nullptr, nullptr, &Image::retrecissement},
		{false, 0, &Image::visionDeuteranopie},
		{false, 0, &Image::visionProtanopie},
		{false, 0, &Image::visionTritanopie},
		{false, 3, nullptr, nullptr, nullptr, FLOUG3},
		{false, 3, nullptr, nullptr, nullptr, FLOUG5},
		{false, 0, &Image::contourSobel},
		{false, 3, nullptr, nullptr, nullptr, CONTRASTER},
		{false, 0, &Image::reglageAuto},
		{false, 0, &Image::reglageAutoGris},
		{false, 0, &Image::reglageAutoCouleur},
	};
}

void VisualIDK::Update() {
	if (!noModif) {
		post = original;
		for (size_t i = 0; i < effects.size(); ++i) {
			if (effects[i].active) {
				switch (effects[i].which)
				{
				case 0:
					post = (post.*(effects[i].func))();
					break;
				
				case 1:
					post = (post.*(effects[i].funcFloat))(effects[i].argFloat);
					break;

				case 2:
					post = (post.*(effects[i].funcUINT32T))(effects[i].argUint32);
					break;

				case 3:
					post = Filtre(effects[i].argFilter).application(post);
					break;
				
				default:
					break;
				}
			}
		}
	}

	#ifndef IDE_ANTI_ERROR
	if (mainMenu[0].active && mainMenu[0].buttons[1].active) {
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

		original = Image(ofn.lpstrFile);
		post = original;
		#elif __linux__
			fileDialog.Open();

		#endif
		mainMenu[0].buttons[1].active = false;
	}
	#endif
	#ifdef __linux__
	if (mainMenu[0].active && mainMenu[0].buttons[2].active) {
		fileDialogSave.Open();
	}

	if(fileDialog.HasSelected())
	{
		original = Image(fileDialog.GetSelected().string());
		post = original;
		noModif = false;
		fileDialog.ClearSelected();
	}

	if(fileDialogSave.HasSelected())
	{
		post.ecrire(fileDialogSave.GetSelected().string());
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
		zoom += io->MouseWheel/10.f;

}

void VisualIDK::Draw() {
	#ifdef USE_DUMB_DRAW
	for (uint32_t y = 0; y < post.getHauteur(); ++y) {
		for (uint32_t x = 0; x < post.getLongueur(); ++x) {
			dl->AddRectFilled({x*pixelSize,y*pixelSize}, {x*pixelSize+pixelSize, y*pixelSize+pixelSize},
			IM_COL32(post.img.r[x][y], post.img.v[x][y], post.img.b[x][y], 255));
		}
	}
	#else

	// Interleave RGB values
	if (!noModif) {
		width = post.getLongueur();
		height = post.getHauteur();
		noModif = true;
		if (width != 0 && height != 0) {
			imageData.clear();
			for (int y = 0; y < height; ++y) {
				for (int x = 0; x < width; ++x) {
					imageData.push_back(static_cast<unsigned char>(post.img.r[x][y]));
					imageData.push_back(static_cast<unsigned char>(post.img.v[x][y]));
					imageData.push_back(static_cast<unsigned char>(post.img.b[x][y]));
					imageData.push_back(static_cast<unsigned char>(255));
				}
			}

			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, post.img.r.size(), post.img.r[0].size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData.data());

			// Set texture parameters (you may need to adjust these based on your requirements)
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			
			// Use the shader program, bind the texture, and draw a quad
			glBindTexture(GL_TEXTURE_2D, textureID);
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
	if (width != 0 && height != 0)
		ImGui::Image(
			(void*)(intptr_t)textureID,
			ImVec2(width * zoom, height * zoom)
		);
	else
		cout << "Empty image" << endl;
	#endif //USE_DUMB_DRAW
	ImGui::End();
	fileDialog.Display();
	fileDialogSave.Display();
}

void VisualIDK::UI() {
	
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
		ImGui::SameLine();
		ImGui::Text("FPS: %1.f", 1.f/io->DeltaTime);
		if (mainMenu[2].buttons[0].active) {
			ImGui::Text("Partie 1 - NBGC");
			ImGui::Checkbox("Noir & Blanc", &effects[EFFECTS_noirBlanc].active);
			ImGui::Checkbox("Composante Rouge", &effects[EFFECTS_composantRouge].active);
			ImGui::Checkbox("Niveaux Gris", &effects[EFFECTS_niveauxGris].active);
			ImGui::Checkbox("Luminosité", &effects[EFFECTS_luminosity].active);
			ImGui::SameLine();
			ImGui::PushItemWidth(96);
			ImGui::InputFloat("l", &effects[EFFECTS_luminosity].argFloat, 0.1f, 0.2f);
			ImGui::PopItemWidth();
			ImGui::Checkbox("Contraste", &effects[EFFECTS_contraste].active);
			ImGui::SameLine();
			ImGui::PushItemWidth(96);
			ImGui::InputFloat("c", &effects[EFFECTS_contraste].argFloat, 0.1f, 0.2f);
			ImGui::PopItemWidth();
			ImGui::Text("Daltonisme:");
			ImGui::SameLine();
			ImGui::Checkbox("T", &effects[EFFECTS_visionTritanopie].active);
			ImGui::SameLine();
			ImGui::Checkbox("P", &effects[EFFECTS_visionProtanopie].active);
			ImGui::SameLine();
			ImGui::Checkbox("De", &effects[EFFECTS_visionDeuteranopie].active);
			ImGui::Checkbox("reglageAuto", &effects[EFFECTS_reglageAuto].active);
			ImGui::NewLine();
		}
		if (mainMenu[2].buttons[1].active) {
			ImGui::Text("Partie 3 - Géométrie");
			ImGui::Checkbox("Rotation Droite", &effects[EFFECTS_rotationD].active);
			ImGui::Checkbox("Rotation Gauche", &effects[EFFECTS_rotationG].active);
			ImGui::Text("Retournement");
			ImGui::SameLine();
			ImGui::Checkbox("Ho", &effects[EFFECTS_retournementH].active);
			ImGui::SameLine();
			ImGui::Checkbox("Ve", &effects[EFFECTS_retournementV].active);
			ImGui::Text("Rognement");
			ImGui::SameLine();
			ImGui::Checkbox("D", &effects[EFFECTS_rognerD].active);
			ImGui::SameLine();
			ImGui::Checkbox("G", &effects[EFFECTS_rognerG].active);
			ImGui::SameLine();
			ImGui::Checkbox("H", &effects[EFFECTS_rognerH].active);
			ImGui::SameLine();
			ImGui::Checkbox("B", &effects[EFFECTS_rognerB].active);
			ImGui::Checkbox("Agrandissement", &effects[EFFECTS_agrandissement].active);
			ImGui::SameLine();
			ImGui::PushItemWidth(64);
			ImGui::InputInt("aV", (int*)&effects[EFFECTS_agrandissement].argUint32, 1, 2);
			ImGui::PopItemWidth();
			ImGui::Checkbox("Retrécisssement", &effects[EFFECTS_retrecissement].active);
			ImGui::SameLine();
			ImGui::PushItemWidth(64);
			ImGui::InputInt("rV", (int*)&effects[EFFECTS_luminosity].argUint32, 1, 2);
			ImGui::PopItemWidth();
			ImGui::NewLine();
		}
		if (mainMenu[2].buttons[2].active) {
			ImGui::Text("Partie 4 - Filtres");
			ImGui::Checkbox("FlouG3", &effects[EFFECTS_filtreFlouG3].active);
			ImGui::Checkbox("FlouG5", &effects[EFFECTS_filtreFlouG5].active);
			ImGui::Checkbox("Contour Sobel", &effects[EFFECTS_filtreContourSobel].active);
			ImGui::Checkbox("Contraster", &effects[EFFECTS_filtreContraster].active);
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