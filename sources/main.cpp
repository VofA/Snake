#include <iostream>
#include <ncurses.h>
#include <string>

/*

Обработка нажатий клавиш
Проверка столкновения
Проверка съедания еды
Создание еды
Отрисовка карты

*/

struct sPoint {
  int x = 0;
  int y = 0;
};
sPoint food;

enum sDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };
struct sPlayer {
  sDirection direction;
  std::vector<sPoint> position;
};

sPlayer player;

const int width  = 20;
const int height = 10;
unsigned int score;

void zeroGame() {
  std::vector<int> position;
  player.position player.x = width / 2;
  player.y                 = height / 2;
  food.x                   = rand() % width;
  food.y                   = rand() % height;
  score                    = 0;
}

void foodGenerate() {
}

void draw(int coordX, int coordY) {
  clear();

  for (int i = 0; i < width + 2; ++i) {
    std::cout << "#";
  }
  std::cout << "\n";

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      if (coordX == x && coordY == y) {
        if (y == coordY && x == coordX) {
          std::cout << "*";
        } else if (y == fruitY && x == fruitX) {
          std::cout << "F";
        } else {
          std::cout << " ";
        }
        if (x == 0) {
          std::cout << "#";
        }
      }

      if (x == width - 1) {
        std::cout << "#";
      }
    }
    std::cout << "\n";
  }

  for (int i = 0; i < width + 2; ++i) {
    std::cout << "#";
  }
  std::cout << "\n";
}
void input() {
}

void logic() {
}

int main() {
  srand((unsigned)time(NULL));

  setup();
  draw(1, 1);
  while (!gameOver) {

    initscr();
    char key = getch();

    int relativeX = 0;
    int relativeY = 0;

    switch (key) {
      case 'w':
        relativeY--;
        break;
      case 'a':
        relativeX--;
        break;
      case 's':
        relativeY++;
        break;
      case 'd':
        relativeX++;
        break;
      case (char)10:
        gameOver = true;
        break;
    }

    if (x + relativeX >= 0 && x + relativeX <= width - 1 &&
        y + relativeY >= 0 && y + relativeY <= height - 1) {
      y += relativeY;
      x += relativeX;
    }
    draw(1, 1);
  }
  endwin();

  return 0;
}