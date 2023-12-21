#include "ui.hpp"

UI::UI() {
	tabs = std::vector<ImageTab>(0);
	mainMenu.push_back((MainMenuDropdown){"File", false, vector<MainMenuDropdownButton>{{"New", false}, {"Load", false}, {"Save", false}}});
	mainMenu.push_back((MainMenuDropdown){"Edit", false, vector<MainMenuDropdownButton>{{"Cut", false}, {"Copy", false}, {"Paste", false}}});
	mainMenu.push_back((MainMenuDropdown){"Display", false, vector<MainMenuDropdownButton>{{"NBGC", false}, {"Géométrie", false}, {"Filtres", false}, {"Toggle dark mode", false}}});
	mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_NBGC	].active = true;
	mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_GEO		].active = true;
	mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_FILTER	].active = true;

	MenuBarSize = {0, 0, -1, (18+30)};
	ImageSize = {64, (18+30), 1280-240-64, 720-(18+30)-16};
	EditingSize = {1280 - 240, (18+30), 240, 720 - (18+30)};
	ToolbarSize = {0, (18+30), 64, 720 - (18+30)};
}

void UI::Update() {
	Tabbar();
	Toolbar();
	Editing();
	MenuBar();
	ErrorBay();
	DarkMode();

	hasSetDefaultSizes = true;

}
void UI::initEffects(size_t tabIndex) {
	tabs[tabIndex].effects = {
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
	tabs[tabIndex].effectsCopy = tabs[tabIndex].effects;
}

void UI::MenuBar() {
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

	//If User pressed CTRL+N or File->New.
	if (mainMenu[MENU_FILE].buttons[MENU_FILE_NEW].active || (ImGui::IsKeyPressed(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_N))) {
		tabs.push_back(ImageTab());
		initEffects(tabs.size()-1);
		mainMenu[MENU_FILE].buttons[MENU_FILE_NEW].active = false;
	}
}

void UI::Editing() {
	if (mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_NBGC].active || mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_GEO].active || mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_FILTER].active) {
		ImGui::Begin("Editing");
		if (!hasSetDefaultSizes) {
			ImGui::SetWindowPos({EditingSize.x, EditingSize.y});
			ImGui::SetWindowSize({EditingSize.z, EditingSize.w});
		}
		//EditingSize = ImVec4(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		ImGui::SetWindowPos({io->DisplaySize.x-EditingSize.z, EditingSize.y});
		ImGui::SetWindowSize({ ImGui::GetWindowSize().x, io->DisplaySize.y - EditingSize.y});
		//if (ImGui::Button("Refresh")) noModif = false;
		ImGui::SameLine(0.f);
		ImGui::Text("FPS: %1.f", 1.f/io->DeltaTime);
		if (mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_NBGC].active) {
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
		if (mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_GEO].active) {
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
		if (mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_FILTER].active) {
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

void UI::Toolbar() {
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

void UI::ErrorBay() {
	ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0.f, 0.f));
	ImGui::Begin("ErrorTab", (bool*)__null, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::Text("%s", tabs[current_tab].post.getError().c_str() );
	ImGui::SetWindowPos({ToolbarSize.z, io->DisplaySize.y-32});
	ImGui::SetWindowSize(ImVec2(io->DisplaySize.x - EditingSize.z - ToolbarSize.z, 32));
	ImGui::End();
	ImGui::PopStyleVar();
}

void UI::DarkMode() {
	if (mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_DARKMODE].active) {
		disableAutoDarkMode = true;
		darkMode = !darkMode;
		mainMenu[MENU_DISPLAY].buttons[MENU_DISPLAY_DARKMODE].active = false;
	}
	if (!disableAutoDarkMode) {
		darkMode = !WindowsUtils::isLightTheme();
	}
	if (darkMode) {
		ImGui::StyleColorsDark();
		clear_color = ImVec4(0.f, 0.f, 0.f, 1.00f);

	} else {
		ImGui::StyleColorsLight();
		clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	}
}

void UI::Tabbar() {
	ImGui::Begin("debtabs", (bool*)__null, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::BeginTabBar("Tabsa", ImGuiTabBarFlags_NoTooltip);
	ImGui::SetWindowPos({0, 18});
	ImGui::SetWindowSize({io->DisplaySize.x, 30});
	
	for (size_t i = 0; i < tabs.size(); ++i) {
		if (current_tab == i) 
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, (darkMode) ? 1 : 0, 1, 1));
		bool hasClicked = ImGui::TabItemButton((tabs[current_tab].title + to_string(i)).c_str());
		if (hasClicked)	
			current_tab = i;
		if (current_tab == i && !hasClicked)
			ImGui::PopStyleColor();
	}
	
	if(ImGui::TabItemButton("+")) {
		tabs.push_back(ImageTab());
		initEffects(tabs.size()-1);
	}
	ImGui::EndTabBar();
	ImGui::End();
}