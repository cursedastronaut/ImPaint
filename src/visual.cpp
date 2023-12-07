#include "visual.h"

VisualIDK::VisualIDK() {
	post = original;
}

void VisualIDK::Update() {
	if (!noModif) {
		post = original;
		if (noirBlanc)
			post = post.noirEtBlanc();
		if (composantRouge)
			post = post.composanteRouge();
		if (niveauxGris)
			post = post.niveauxGris();
		if (luminosity)
			post = post.changeLuminosity(luminosityF);
		if (contraste)
			post = post.contrasteUp(contrasteF);
		if (rotationD)
			post = post.rotationD();
		if (rotationG)
			post = post.rotationG();
		if (retournementH)
			post = post.retournementH();
		if (retournementV)
			post = post.retournementV();
		if (rognerD)
			post = post.rognerD(1);
		if (rognerG)
			post = post.rognerG(1);
		if (rognerH)
			post = post.rognerH(1);
		if (rognerB)
			post = post.rognerB(1);
		if (agrandissement)
			post = post.agrandissement(agrandissementV);
		if (retrecissement)
			post = post.retrecissement(retrecissementV);
		if (visionDeuteranopie)
			post = post.visionDeuteranopie();
		if (visionProtanopie)
			post = post.visionProtanopie();
		if (visionTritanopie)
			post = post.visionTritanopie();
		if (filtreFlouG3) {
			Filtre flouG3(FLOUG3);
			post = flouG3.application(post);
		}
		if (filtreFlouG5) {
			Filtre flouG5(FLOUG5);
			post = flouG5.application(post);
		}
	}
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
	
	if (width != 0 && height != 0)
		dl->AddImage((void*)(intptr_t)textureID, ImVec2(0, 0), ImVec2(height, width));
	else
		cout << "Empty image" << endl;
	
	
	#endif //USE_DUMB_DRAW
}

void VisualIDK::UI() {
	ImGui::Begin("Debug");
	if (ImGui::Button("Refresh")) noModif = false;
	ImGui::SameLine();
	ImGui::Text("FPS: %1.f", 1.f/io->DeltaTime);
	ImGui::Text("Partie 1 - NBGC");
	ImGui::Checkbox("Noir & Blanc", &noirBlanc);
	ImGui::Checkbox("Composante Rouge", &composantRouge);
	ImGui::Checkbox("Niveaux Gris", &niveauxGris);
	ImGui::Checkbox("Luminosité", &luminosity);
	ImGui::SameLine();
	ImGui::PushItemWidth(96);
	ImGui::InputFloat("l", &luminosityF, 0.1f, 0.2f);
	ImGui::PopItemWidth();
	ImGui::Checkbox("Contraste", &contraste);
	ImGui::SameLine();
	ImGui::PushItemWidth(96);
	ImGui::InputFloat("c", &contrasteF, 0.1f, 0.2f);
	ImGui::PopItemWidth();
	ImGui::Text("Daltonisme:");
	ImGui::SameLine();
	ImGui::Checkbox("T", &visionTritanopie);
	ImGui::SameLine();
	ImGui::Checkbox("P", &visionProtanopie);
	ImGui::SameLine();
	ImGui::Checkbox("De", &visionDeuteranopie);
	ImGui::NewLine();
	ImGui::Text("Partie 3 - Géométrie");
	ImGui::Checkbox("Rotation Droite", &rotationD);
	ImGui::Checkbox("Rotation Gauche", &rotationG);
	ImGui::Text("Retournement");
	ImGui::SameLine();
	ImGui::Checkbox("Ho", &retournementH);
	ImGui::SameLine();
	ImGui::Checkbox("Ve", &retournementV);
	ImGui::Text("Rognement");
	ImGui::SameLine();
	ImGui::Checkbox("D", &rognerD);
	ImGui::SameLine();
	ImGui::Checkbox("G", &rognerG);
	ImGui::SameLine();
	ImGui::Checkbox("H", &rognerH);
	ImGui::SameLine();
	ImGui::Checkbox("B", &rognerB);
	ImGui::Checkbox("Agrandissement", &agrandissement);
	ImGui::SameLine();
	ImGui::PushItemWidth(64);
	ImGui::InputInt("aV", &agrandissementV, 1, 2);
	ImGui::PopItemWidth();
	ImGui::Checkbox("Retrécisssement", &retrecissement);
	ImGui::SameLine();
	ImGui::PushItemWidth(64);
	ImGui::InputInt("rV", &retrecissementV, 1, 2);
	ImGui::PopItemWidth();
	ImGui::Checkbox("No Refresh", &dontRefresh);
	ImGui::NewLine();
	ImGui::Text("Partie 4 - Filtres");
	ImGui::Checkbox("FlouG3", &filtreFlouG3);
	ImGui::Checkbox("FlouG5", &filtreFlouG5);
	ImGui::NewLine();
	ImGui::Text("Partie 2 - Fichiers");
	ImGui::InputText("File", fileTempBuffer, 50);
	if (ImGui::Button("Charger", {64, 32}))
		original = Image(fileTempBuffer);
	#ifdef USE_DUMB_DRAW
	ImGui::InputInt("Pixel Size", &pixelSize, 1, 2);
	#endif //USE_DUMB_DRAW
	ImGui::SameLine();
	if (ImGui::Button("Écrire", {64, 32})) {
		post.ecrire();
		std::cout << "Ecriture terminee" << std::endl;
		noModif = false;
	}
	ImGui::End();
}