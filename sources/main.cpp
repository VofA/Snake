#include <chrono>
#include <deque>
#include <iostream>
#include <ncurses.h>
#include <string>
#include <thread>
#include <vector>

struct sPoint {
  int x       = 0;
  int y       = 0;
  char symbol = ' ';
};

enum sDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };

struct sPlayer {
  sDirection direction;
  std::deque<sPoint> position;
};

sPlayer player;
sPoint food;

const int width  = 20;
const int height = 10;

unsigned int score    = 0;
unsigned int maxScore = 0;

const unsigned int tickTime = 150;

bool foodCheck() {
  for (const auto &i : player.position) {
    if (i.x == food.x && i.y == food.y) {
      return true;
    }
  }
  return false;
}

void foodGenerate() {
  do {
    food.x = rand() % (width - 1) + 1;
    food.y = rand() % (height - 1) + 1;
  } while (foodCheck());
}

void zeroGame() {
  food.symbol      = '~';
  player.direction = STOP;

  player.position.clear();

  sPoint head{width / 2, height / 2, 'O'};
  player.position.push_front(head);

  if (maxScore < score) {
    maxScore = score;
  }

  score = 0;

  foodGenerate();
}

bool checkCollision() {
  for (const auto &i : player.position) {
    if (i.x < 1 || i.x > width - 1 || i.y < 1 || i.y > height - 1) {
      return true;
    }
  }
  for (auto i = player.position.begin() + 1; i < player.position.end(); ++i) {
    if (player.position[0].x == i->x && player.position[0].y == i->y) {
      return true;
    }
  }
  return false;
}

void drawHandler() {
  while (true) {
    clear();

    for (int y = 0; y <= height; ++y) {
      mvprintw(y, 0, "#");
      mvprintw(y, width, "#");
    }

    for (int x = 0; x <= width; ++x) {
      mvprintw(0, x, "#");
      mvprintw(height, x, "#");
    }

    for (const auto &i : player.position) {
      mvprintw(i.y, i.x, "%c", i.symbol);
    }

    mvprintw(food.y, food.x, "%c", food.symbol);

    mvprintw(height + 1, 0, "Score: %u", score);
    mvprintw(height + 2, 0, "Max score: %u", maxScore);
    move(height + 3, 0);

    refresh();
    std::this_thread::sleep_for(std::chrono::milliseconds(tickTime));
  }
}

void inputHandler() {
  while (true) {
    switch (getch()) {
      case 'w':
        player.direction = player.direction == DOWN ? player.direction : UP;
        break;
      case 'a':
        player.direction = player.direction == RIGHT ? player.direction : LEFT;
        break;
      case 's':
        player.direction = player.direction == UP ? player.direction : DOWN;
        break;
      case 'd':
        player.direction = player.direction == LEFT ? player.direction : RIGHT;
        break;
    }
  }
}

int main() {
  srand((unsigned)time(NULL));
  initscr();
  noecho();

  zeroGame();

  std::thread threadInputHandler(inputHandler);
  threadInputHandler.detach();

  std::thread threadDrawHandler(drawHandler);
  threadDrawHandler.detach();

  while (true) {
    sPoint relative;
    sPoint newHead;

    switch (player.direction) {
      case UP:
        newHead.symbol = 'A';
        relative.y--;
        break;
      case LEFT:
        newHead.symbol = '<';
        relative.x--;
        break;
      case DOWN:
        newHead.symbol = 'V';
        relative.y++;
        break;
      case RIGHT:
        newHead.symbol = '>';
        relative.x++;
        break;
      case STOP:
        continue;
    }

    newHead.x = player.position.front().x + relative.x;
    newHead.y = player.position.front().y + relative.y;
    player.position.push_front(newHead);

    if (checkCollision()) {
      zeroGame();
      continue;
    }

    if (foodCheck()) {
      score++;
      foodGenerate();
    } else {
      player.position.pop_back();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(tickTime));
  }

  endwin();

  return 0;
}