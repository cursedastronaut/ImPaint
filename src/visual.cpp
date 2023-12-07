#include "visual.h"

VisualIDK::VisualIDK() {
}

void VisualIDK::Update() {
	if (!dontRefresh) {
		post = original;
	} else {
		noirBlanc = false;
		composantRouge = false;
		niveauxGris = false;
		luminosity = false;
		contraste = false;
		rotationD = false;
		rotationG = false;
		retournementH = false;
		retournementV = false;
		rognerD = false;
		rognerB = false;
		rognerG = false;
		rognerH = false;
		agrandissement = false;
	}
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
		post = post.agrandissement(2);
	if (retrecissement)
		post = post.retrecissement(2);
	if (visionDeuteranopie)
		post = post.visionDeuteranopie();
	if (visionProtanopie)
		post = post.visionProtanopie();
	if (visionTritanopie)
		post = post.visionTritanopie();

}

void VisualIDK::Draw() {
	for (uint32_t y = 0; y < post.getHauteur(); ++y) {
		for (uint32_t x = 0; x < post.getLongueur(); ++x) {
			dl->AddRectFilled({x*pixelSize,y*pixelSize}, {x*pixelSize+pixelSize, y*pixelSize+pixelSize},
			IM_COL32(post.img.r[x][y], post.img.v[x][y], post.img.b[x][y], 255));
		}
	}
}

void VisualIDK::UI() {
	ImGui::Begin("Debug");
	ImGui::Text("Partie 1 - NBGC");
	ImGui::Checkbox("Noir & Blanc", &noirBlanc);
	ImGui::Checkbox("Composante Rouge", &composantRouge);
	ImGui::Checkbox("Niveaux Gris", &niveauxGris);
	ImGui::Checkbox("Luminosité", &luminosity);
	ImGui::InputFloat("l", &luminosityF, 0.1f, 0.2f);
	ImGui::Checkbox("Contraste", &contraste);
	ImGui::InputFloat("c", &contrasteF, 0.1f, 0.2f);
	ImGui::Checkbox("Tritanope", &visionTritanopie);
	ImGui::Checkbox("Protanope", &visionProtanopie);
	ImGui::Checkbox("Deutéranope", &visionDeuteranopie);
	ImGui::NewLine();
	ImGui::Text("Partie 3 - Géométrie");
	ImGui::Checkbox("Rotation Droite", &rotationD);
	ImGui::Checkbox("Rotation Gauche", &rotationG);
	ImGui::Checkbox("Retournement Horizontal", &retournementH);
	ImGui::Checkbox("Retournement Vertical", &retournementV);
	ImGui::Checkbox("Rognement Droite", &rognerD);
	ImGui::Checkbox("Rognement Gauche", &rognerG);
	ImGui::Checkbox("Rognement Haut", &rognerH);
	ImGui::Checkbox("Rognement Bas", &rognerB);
	ImGui::Checkbox("Agrandissement", &agrandissement);
	ImGui::Checkbox("Retrécisssement", &retrecissement);
	ImGui::Checkbox("No Refresh", &dontRefresh);
	ImGui::NewLine();
	ImGui::Text("Partie 2 - Fichiers");
	ImGui::InputText("File", fileTempBuffer, 50);
	if (ImGui::Button("Charger", {64, 32}))
		original = Image(fileTempBuffer);
	ImGui::InputInt("Pixel Size", &pixelSize, 1, 2);
	if (ImGui::Button("Écrire", {64, 32})) {
		post.ecrire();
		std::cout << "Ecriture terminee" << std::endl;
	}
	ImGui::End();
}