#include <cmath>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include "ConsoleHandler.hpp"

struct TPOINT {
	int x = 0;
	int y = 0;
};

ConsoleHandler handler;

void handlerKey(int key);
void handlerLogic();
bool playerCheckCollision(int x, int y);
void playerMove(int x, int y);
bool foodCheck();
void foodGenerate();
void lose();
void drawMap();
void drawScore();

TPOINT getPoint(int x, int y);

TPOINT food;
std::vector<TPOINT> playerPosition;
std::vector<std::string> map;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
int playerDirection = RIGHT;

int tickTime = 200;
int score = 0;
int maxScore = 0;

int main() {
	srand((unsigned int)time(NULL));

	std::fstream fin("map");

	while (!fin.eof()) {
		map.push_back("");
		getline(fin, map.back());
	}

	handler.init((unsigned int)map.size() + 1, (unsigned int)map[0].size());
	handler.setTitle(L"Snake");

	drawMap();
	drawScore();

	handler.registerHandlerCallback(handlerKey);
	handler.registerKeyCallback(ConsoleHandler::KEY_CODE_W);
	handler.registerKeyCallback(ConsoleHandler::KEY_CODE_A);
	handler.registerKeyCallback(ConsoleHandler::KEY_CODE_S);
	handler.registerKeyCallback(ConsoleHandler::KEY_CODE_D);

	playerPosition.push_back(getPoint(1, 1));
	foodGenerate();

	std::thread threadLogic(handlerLogic);
	threadLogic.detach();

	handler.mainLoop();
}

void handlerKey(int key) {
	switch (key) {
	case ConsoleHandler::KEY_CODE_W:
		if (playerDirection != DOWN) {
			playerDirection = UP;
		}
		break;
	case ConsoleHandler::KEY_CODE_S:
		if (playerDirection != UP) {
			playerDirection = DOWN;
		}
		break;
	case ConsoleHandler::KEY_CODE_A:
		if (playerDirection != RIGHT) {
			playerDirection = LEFT;
		}
		break;
	case ConsoleHandler::KEY_CODE_D:
		if (playerDirection != LEFT) {
			playerDirection = RIGHT;
		}
		break;
	}
}

void handlerLogic() {
	while (true) {
		int relativeX = 0;
		int relativeY = 0;

		switch (playerDirection) {
		case UP:
			relativeY = -1;
			break;
		case DOWN:
			relativeY = 1;
			break;
		case LEFT:
			relativeX = -1;
			break;
		case RIGHT:
			relativeX = 1;
			break;
		}

		if (!playerCheckCollision(relativeX, relativeY)) {
			playerMove(relativeX, relativeY);
		}
		else {
			lose();
		}

		Sleep(tickTime);
	}
}

bool playerCheckCollision(int x, int y) {
	for (unsigned int i = 0; i < playerPosition.size(); i++) {
		char symbol = map[y + playerPosition[i].y][x + playerPosition[i].x];

		if (symbol == '=') {
			return true;
		}
		else if (i != 0 && (x + playerPosition[0].x) == playerPosition[i].x &&
			(y + playerPosition[0].y) == playerPosition[i].y) {
			return true;
		}
	}
	return false;
}

void playerMove(int x, int y) {
	playerPosition.insert(
		playerPosition.begin(),
		getPoint(playerPosition[0].x + x, playerPosition[0].y + y));
	char symbol;
	switch (playerDirection) {
	case UP:
		symbol = 'A';
		break;
	case DOWN:
		symbol = 'V';
		break;
	case LEFT:
		symbol = '<';
		break;
	case RIGHT:
		symbol = '>';
		break;
	}

	handler.changePixel(playerPosition.front().x, playerPosition.front().y, symbol, Style::create(Color::LIGHTBLUE, Color::WHITE));

	if (!foodCheck()) {
		handler.changePixel(playerPosition.back().x, playerPosition.back().y, ' ', Style::create(Color::WHITE, Color::WHITE));
		playerPosition.pop_back();
	}
}

bool foodCheck() {
	for (unsigned int i = 0; i < playerPosition.size(); i++) {
		if (playerPosition[i].x == food.x && playerPosition[i].y == food.y) {
			handler.changePixel(food.x, food.y, 'o', Style::create(Color::LIGHTBLUE, Color::WHITE));

			score++;
			drawScore();

			foodGenerate();

			return true;
		}
	}
	return false;
}

void foodGenerate() {
	int randX = rand() % (map[0].length() - 2) + 1;
	int randY = rand() % (map.size() - 2) + 1;

	food = getPoint(randX, randY);
	handler.changePixel(randX, randY, '*', Style::create(Color::LIGHTBLUE, Color::WHITE));

	foodCheck();
}

void lose() {
	if (maxScore < score) {
		maxScore = score;
	}

	playerPosition.clear();
	playerPosition.push_back(getPoint(1, 1));
	playerDirection = RIGHT;

	score = 0;

	drawMap();
	drawScore();

	foodGenerate();
}

TPOINT getPoint(int x, int y) {
	TPOINT result;

	result.x = x;
	result.y = y;

	return result;
}

void drawMap() {
	for (unsigned int y = 0; y < map.size(); y++) {
		for (unsigned int x = 0; x < map[y].size(); x++) {
			if (map[y][x] == '=') {
				handler.changePixel(x, y, '=', Style::create(Color::BLACK, Color::WHITE));
			} else {
				handler.changePixel(x, y, map[y][x], Style::create(Color::WHITE, Color::WHITE));
			}
		}
	}
}

void drawScore() {
	std::wstringstream scoreStream;
	scoreStream << "Score: " << score;
	handler.writeText(0, 7, scoreStream.str(), 32, Style::create(Color::BLACK, Color::WHITE));
}