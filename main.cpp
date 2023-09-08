#include <iostream>
#include "input.h"
#include <vector>
#include <unistd.h>

const int MAP_WIDTH = 40;
const int MAP_HEIGHT = 15;

const int INIT_SNAKE_POS = 7;

const int DIR_LEFT = 1;
const int DIR_RIGHT = 2;
const int DIR_UP = 3;
const int DIR_DOWN = 4;

const char FOOD_CHARS[]{'!', '$', '%', '*'};
int currentChar;
int foodX = 2 + rand() % (MAP_WIDTH - 2); // start with random foodCoord
int foodY = 2 + rand() % (MAP_HEIGHT - 2);

int direction = DIR_RIGHT; // init snake direction
int snakeSize = 5;

int score = 0;

struct snakeBody {
    int x;
    int y;
};

std::vector<snakeBody> snake;

bool isSnakeBody(int x, int y) {
    for (int i = 0; i < snakeSize; ++i) {
        if (snake[i].x == x && snake[i].y == y) {
            return true;
        }
    }
    return false;
}

bool isMapBorder(int x, int y) {
    return (x == 0 || x == MAP_WIDTH || y == 0 || y == MAP_HEIGHT);

}

bool isFood(int x, int y) {
    return x == foodX && y == foodY;
}

bool ateFood() {
    return (snake[0].x == foodX && snake[0].y == foodY);
}

bool ateItself() {
    for (int i = 1; i < snakeSize; ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            return true;
        }
    }
    return false;
}

bool hitBorder() {
    return (snake[0].x == 0 || snake[0].y == 0 || snake[0].x == MAP_WIDTH || snake[0].y == MAP_HEIGHT);
}

void generateFoodCoord() {
    while (isSnakeBody(foodX, foodY)) {
        foodX = 2 + rand() % (MAP_WIDTH - 2);
        foodY = 2 + rand() % (MAP_HEIGHT - 2);
    }
}

void generateFoodChar() {
    currentChar = rand() % (sizeof FOOD_CHARS);
}

void initSnake() {
    for (int i = 0; i < snakeSize; ++i) {
        snakeBody s{};
        s.x = INIT_SNAKE_POS - i;
        s.y = INIT_SNAKE_POS;

        snake.push_back(s);
    }
}

void moveSnake() {
    int x = snake[0].x;
    int y = snake[0].y;
    int lastX, lastY;

    switch (direction) { // MOVE SNAKE'S HEAD
        case DIR_LEFT:
            snake[0].x--;
            break;
        case DIR_RIGHT:
            snake[0].x++;
            break;
        case DIR_UP:
            snake[0].y--;
            break;
        default:
            snake[0].y++;
            break;
    }

    for (int i = 1; i < snakeSize; ++i) { // MOVE SNAKE'S BODY
        lastX = snake[i].x;
        lastY = snake[i].y;

        snake[i].x = x;
        snake[i].y = y;

        x = lastX;
        y = lastY;
    }
}

void growSnake() {
    snakeBody piece{};
    piece.x = snake[snakeSize - 1].x;
    piece.y = snake[snakeSize - 1].y;
    snake.push_back(piece);
    snakeSize++;
}

void draw() {
    system("clear"); // so that there are not multiple windows of the game
    for (int i = 0; i <= MAP_HEIGHT; ++i) {
        for (int j = 0; j <= MAP_WIDTH; ++j) {
            if (isMapBorder(j, i)) {
                std::cout << '#' << std::flush; // flush increases performance (input is not buffered)
            } else if (isSnakeBody(j, i)) {
                std::cout << '4' << std::flush;
            } else if (isFood(j, i)) {
                std::cout << FOOD_CHARS[currentChar] << std::flush;
            } else {
                std::cout << ' ' << std::flush;
            }
        }
        std::cout << std::endl;
    }
}

void exit() {
    restoreOriginalSettings();
    std::cout << "YOU SCORED: " << score << std::endl;
}

void inputDirection() {
    char c;
    std::cin >> c;
    switch (c) {
        case 100:
        case 68:
            if (direction != DIR_LEFT) direction = DIR_RIGHT;
            break;

        case 83:
        case 115:
            if (direction != DIR_UP) direction = DIR_DOWN;
            break;

        case 65:
        case 97:
            if (direction != DIR_RIGHT) direction = DIR_LEFT;
            break;

        case 119:
        case 87:
            if (direction != DIR_DOWN) direction = DIR_UP;
            break;

        case 27:
            exit();
        default:
            break;
    }
}

bool hasStandardInput(fd_set &rfds, timeval &timeout) {
    FD_ZERO(&rfds);
    FD_SET(STDIN_FILENO, &rfds);

    return select(1, &rfds, NULL, NULL, &timeout) > 0 && FD_ISSET(STDIN_FILENO, &rfds);
}


void startGameRound(fd_set &rfds, timeval &timeout) {
    while (true) {
        if (hasStandardInput(rfds, timeout)) {
            inputDirection();
        }
        moveSnake();
        if (hitBorder() || ateItself()) {
            exit();
            break;
        }
        if (ateFood()) {
            score += 5;
            generateFoodCoord();
            generateFoodChar();
            growSnake();
        }
        draw();
        usleep(150000);
    }
}


void init() {
    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    fd_set rfds;
    enableRawInputMode();
    initSnake();
    generateFoodCoord();
    generateFoodChar();
    startGameRound(rfds, timeout);
}


int main() {
    srand(time(nullptr));
    init();
    return 0;
}


