#include <graphics.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>

#define MAX 200

int x[MAX], y[MAX];
int foodx, foody;
int length = 5;
char dir = 'R';
int score = 0;

void drawBoundary() {
    rectangle(10, 10, 630, 470);
}

void generateFood() {
    foodx = (rand() % 60 + 2) * 10;
    foody = (rand() % 40 + 2) * 10;
}

void drawFood() {
    setfillstyle(SOLID_FILL, RED);
    bar(foodx, foody, foodx + 10, foody + 10);
}

void drawSnake() {
    setfillstyle(SOLID_FILL, GREEN);
    for (int i = 0; i < length; i++) {
        bar(x[i], y[i], x[i] + 10, y[i] + 10);
    }
}

void moveSnake() {
    for (int i = length - 1; i > 0; i--) {
        x[i] = x[i - 1];
        y[i] = y[i - 1];
    }

    if (dir == 'R') x[0] += 10;
    if (dir == 'L') x[0] -= 10;
    if (dir == 'U') y[0] -= 10;
    if (dir == 'D') y[0] += 10;
}

int checkCollision() {
    if (x[0] <= 10 || x[0] >= 620 || y[0] <= 10 || y[0] >= 460)
        return 1;

    for (int i = 1; i < length; i++) {
        if (x[0] == x[i] && y[0] == y[i])
            return 1;
    }
    return 0;
}

void checkFood() {
    if (x[0] == foodx && y[0] == foody) {
        length++;
        score += 10;
        generateFood();
    }
}

void showScore() {
    char s[20];
    sprintf(s, "Score: %d", score);
    outtextxy(500, 20, s);
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    // Initial snake position
    for (int i = 0; i < length; i++) {
        x[i] = 100 - i * 10;
        y[i] = 100;
    }

    generateFood();

    while (1) {
        cleardevice();
        drawBoundary();
        drawFood();
        drawSnake();
        showScore();

        if (kbhit()) {
            char key = getch();
            if (key == 75 && dir != 'R') dir = 'L'; // Left
            if (key == 77 && dir != 'L') dir = 'R'; // Right
            if (key == 72 && dir != 'D') dir = 'U'; // Up
            if (key == 80 && dir != 'U') dir = 'D'; // Down
        }

        moveSnake();
        checkFood();

        if (checkCollision()) {
            cleardevice();
            outtextxy(250, 200, "GAME OVER");
            break;
        }

        delay(120);
    }

    getch();
    closegraph();
    return 0;
}
