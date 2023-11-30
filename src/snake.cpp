#include "snake.h"

using namespace std;

//Splits a string into an array of substrings
static void split(std::string in, char splitter, std::vector<std::string>* output);

int toInt(const char* index);
vector<uint32_t> getMultipleInt(string &);
//Snake class constructor
Snake::Snake() {
	updateCap = UPDATE_CAP;
	updateTimer = 0.f;

	tail = {{5.f, 7.f}};
	tailTemp = {-1, -1};
	scene = 0;
	srand(time(NULL)); // Seed the time
	applePos = {
		static_cast<float>(rand() % (int)(GRID_SIZE.x)) ,
		static_cast<float>(rand() % (int)(GRID_SIZE.y)) 
	};
}

//Main update function
void Snake::Update() {
	getDirection();
	if (updateTimer >= updateCap) {
		updateTimer = 0.f;

		previousDirection = direction;
		
		//Go through each tail section, and move them
		for (size_t i = tail.size()-1; i > 0 && i < tail.size(); --i) {
			Movement(i);
		}
		Movement(0);

		//Checks if snake head is beyond limit
		if (!loopAtBorders && (tail[0].x < 0 || tail[0].x >= GRID_SIZE.x || tail[0].y < 0 || tail[0].y >= GRID_SIZE.y))
			gameOver();
		else
			borderLoop();

		//Checks if snake head is colliding with a tail section
		for (size_t i = 1; i < tail.size(); ++i)
			if (tail[0] == tail[i])
				scene = SCENE_GAMEOVER;

		if (tailTemp.x != -1 && tailTemp.y != -1) {
			tail.push_back(tailTemp);
			tailTemp = {-1, -1};
		}

		if (tail[0] == applePos) {
			applePos = {
				static_cast<float>(rand() % (int)(GRID_SIZE.x)) ,
				static_cast<float>(rand() % (int)(GRID_SIZE.y)) 
			};
			++ appleEaten;
			tailTemp = tail[tail.size()-1];
		}
	}
	updateTimer += io->DeltaTime;
	if (debugMode)
		DebugInfo();
}

//Handles snake head and tail movement
ImVec2 Snake::Movement(size_t i) {
	ImVec2 prevPos = tail[i];
	if (i == 0) {
		switch(direction) {
			case DIR_UP:
				tail[i].y --;
				break;
			case DIR_LEFT:
				tail[i].x --;
				break;
			case DIR_DOWN:
				tail[i].y ++;
				break;
			case DIR_RIGHT:
				tail[i].x ++;
				break;
			default:
				break;
		}
	}
	else {
		tail[i] = tail[i-1];
	}

	return prevPos;
}

//Draws the snake, and the apple
void Snake::Draw() {
	for (size_t i = 0; i < tail.size(); ++i)
		dl->AddRectFilled(tail[i]*CASE_SIZE, tail[i]*CASE_SIZE+CASE_SIZE, IM_COL32_WHITE);
	dl->AddRectFilled(applePos*CASE_SIZE, applePos*CASE_SIZE+CASE_SIZE, IM_COL32(255, 0, 0, 255));
	//Draws score
	dl->AddText({0,0}, IM_COL32_BLACK, string("Score: " + to_string(appleEaten)).c_str());
}

//Gets the user input to change to a direction
void Snake::getDirection() {
	if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)		&& (tail.size() < 2 || previousDirection != DIR_DOWN))
		direction = DIR_UP;
	if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)		&& (tail.size() < 2 || previousDirection != DIR_RIGHT))
		direction = DIR_LEFT;
	if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)		&& (tail.size() < 2 || previousDirection != DIR_UP))
		direction = DIR_DOWN;
	if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)	&& (tail.size() < 2 || previousDirection != DIR_LEFT))
		direction = DIR_RIGHT;
}

//Displays debug informations
void Snake::DebugInfo() {
	ImGui::Begin("Debug Informations");
	ImGui::Text("FPS: %1.f", 1.f/io->DeltaTime);
	ImGui::Text("PlaygroundSize: %d, %d", GRID_SIZE.x, GRID_SIZE.y);
	ImGui::Text("Apple position: %1.f, %1.f", applePos.x, applePos.y);
	ImGui::Text("Apple Eaten: %d", appleEaten);
	for (size_t i = 0; i < tail.size(); ++i)
		ImGui::Text("Snake%lld : %1.f, %1.f", i, tail[i].x, tail[i].y);
	ImGui::End();
}

//Resets values of snake object to launch game again.
void Snake::Reset() {
	tail = {{5.f, 7.f}};
	tailTemp = {-1, -1};
	scene = 1;
	srand(time(NULL)); // Seed the time
	applePos = {
		static_cast<float>(rand() % (int)(GRID_SIZE.x)) ,
		static_cast<float>(rand() % (int)(GRID_SIZE.y)) 
	};
}

//Make the snake head loop around the border
void Snake::borderLoop() {
	if (tail[0].x >= GRID_SIZE.x)
		tail[0].x = 0;
	else if (tail[0].y >= GRID_SIZE.y)
		tail[0].y = 0;
	else if (tail[0].x < 0)
		tail[0].x = GRID_SIZE.x - 1;
	else if (tail[0].y < 0)
		tail[0].y = GRID_SIZE.y - 1;
}

//Handles user's command line arguments, returns true if the program should exit.
bool Snake::setUserArgs(const int argc, char* argv[]) {
	for (int i = 0; i < argc; ++i)
	{
		if (strcmp(argv[i], "--loop") == 0) {
			loopAtBorders = true;
		} else if (strcmp(argv[i], "--skip") == 0) {
			scene = 1;
		} else if (strcmp(argv[i], "--debug") == 0) {
			debugMode = true;
		} else if (strcmp(argv[i], "--gridx") == 0) {
			if (i >= argc - 1) {
				cout	<< "Error: --gridx argument is empty." << endl
						<< "Syntax is --gridx [grid horizontal size]" << endl;
				return true;
			} else {
				GRID_SIZE.x = (float)toInt(argv[i+1]);
				if (GRID_SIZE.x == 0) {
					cout << "Erreur: Invalid value at --gridx" << endl;
					return true;
				}
			}
		} else if (strcmp(argv[i], "--gridy") == 0) {
			if (i >= argc - 1) {
				cout	<< "Error: --gridy argument is empty." << endl
						<< "Syntax is --gridy [grid horizontal size]" << endl;
				return true;
			} else {
				GRID_SIZE.y = (float)toInt(argv[i+1]);
				if (GRID_SIZE.y == 0) {
					cout << "Erreur: Invalid value at --gridy" << endl;
					return true;
				}
			}
		} else if (strcmp(argv[i], "--help") == 0) {
			cout	<< "Snake by @cursedastronaut on Github" << endl
					<< "\t- `--loop`\n\t\tAllows player to loop around borders instead of having a game over." << endl
					<< "\t- `--x [width]`\n\t\tAllows player to choose their horizontal grid size. Default is 40." << endl
					<< "\t- `--y [height]`\n\t\tAllows player to choose their vertical grid size. Default is 30." << endl
					<< "\t- `--skip`\n\t\tAllows player to skip the main menu." << endl;
					return true;
		} else if (strcmp(argv[i], "--speed") == 0) {
			if (i >= argc - 1) {
				cout	<< "Error: --speed argument is empty." << endl
						<< "Syntax is --speed [x] (where the speed of the snake is 1/x)" << endl;
				return true;
			} else {
				int temp = toInt(argv[i+1]);
				if (temp == 0) {
					cout << "Erreur: Invalid value at --speed" << endl;
					return true;
				}
				updateCap = 1.f/(float)temp;
			}
		}
	}
	return false;
}

int toInt(const char* index) {
	std::string temp = index;
	if (temp.size() == 0)
		return 0;
	int result = 0;	

	for (size_t i = temp.size()-1; i < temp.size(); --i)
	{
		if (temp[i] < '0' || temp[i] > '9') {
			return 0;
		}
		result += (temp[i] - '0') * std::pow(10, temp.size()-1-i);
	}
	return result;
}

void Snake::gameOver() {
	saveBestScore();
	scene = SCENE_GAMEOVER;
}

//Reads and save the score if it is part of the 10 best saved ones.
void Snake::saveBestScore() {
	//Checks if save.bin exists
	if (filesystem::is_regular_file("save.bin")) {
		//Reading the file part
		fstream saveFile("save.bin", ios::in | ios::out | ios::app | ios::binary);
		if (!saveFile.is_open())
			return;
		
		string fileContent;
		std::getline(saveFile, fileContent);
		vector<uint32_t> scores = getMultipleInt(fileContent);
		
		if (scores.size() != 10)
			return;

		scores.push_back(appleEaten);
		std::sort(scores.begin(), scores.end());

		if (debugMode) {
			for (size_t i = 0; i < scores.size(); ++i)
				cout << scores[i] << ";";
			cout << endl;
		}

		saveFile.close();

		//Writing the save file part
		fstream saveFileWrite("save.bin", ios::out | ios::binary | ios::trunc);
		for ( size_t i = 0; i < scores.size() && i < 10; ++i ) {
			cout << scores[10-i] << (i == 9 || i >= scores.size()-1 ? "" : ",");
			saveFileWrite << scores[10-i] << (i == 9 || i >= scores.size()-1 ? "" : ",");
		}

		saveFileWrite.close();

	} else {
		fstream saveFile("save.bin", ios::out | ios::app | ios::binary);
		if (!saveFile.is_open())
			return;

		saveFile << "0,0,0,0,0,0,0,0,0,0";
		saveFile.close();
		saveBestScore();
	}
}

vector<uint32_t> getMultipleInt(string &save) {
	vector<string> numbers;
	split(save, ',', &numbers);
	
	vector<uint32_t> output;

	for (size_t i = 0; i < numbers.size(); ++i) {
		output.push_back(toInt(numbers[i].c_str()));
	}

	return output;
}

void split(std::string in, char splitter, std::vector<std::string>* output)
{
	size_t startIndex = 0, endIndex = 0;
	for (size_t i = 0; i <= in.size(); i++) {

		// If we reached the end of the word or the end of the input.
		if (in[i] == splitter || i == in.size()) {
			endIndex = i;
			std::string temp;
			temp.append(in, startIndex, endIndex - startIndex);
			output->push_back(temp);
			startIndex = endIndex + 1;
		}
	}
	return;
}