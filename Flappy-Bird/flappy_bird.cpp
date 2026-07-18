#include <graphics.h>
#include <conio.h>
const int screenWidth = 800;
const int screenHeight = 600;


int birdX = 100;
int birdY = 300;
int birdRadius = 15;
int velocity = 0;
int gravity = 1;
int jump = -15;


int pipeWidth = 50;
int pipeGap = 150;
int pipeX = screenWidth;
int pipeHeight = rand() % 300 + 100;


int score = 0;


void drawBird() {
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    circle(birdX, birdY, birdRadius);
    floodfill(birdX, birdY, YELLOW);
}


void drawPipe() {
    setcolor(GREEN);
    setfillstyle(SOLID_FILL, GREEN);
    
    rectangle(pipeX, 0, pipeX + pipeWidth, pipeHeight);
    floodfill(pipeX + 1, pipeHeight - 1, GREEN);
  
    rectangle(pipeX, pipeHeight + pipeGap, pipeX + pipeWidth, screenHeight);
    floodfill(pipeX + 1, pipeHeight + pipeGap + 1, GREEN);
}


bool checkCollision() {
    if (birdY - birdRadius <= 0 || birdY + birdRadius >= screenHeight) {
        return true; 
    }
    if (birdX + birdRadius >= pipeX && birdX - birdRadius <= pipeX + pipeWidth) {
        if (birdY - birdRadius <= pipeHeight || birdY + birdRadius >= pipeHeight + pipeGap) {
            return true; 
        }
    }
    return false;
}

int main() {
    int gd = DETECT, gm;
    initgraph(&gd, &gm, "");

    while (true) {
        cleardevice();

        
        drawBird();
        drawPipe();

       
        pipeX -= 5;
        if (pipeX + pipeWidth < 0) {
            pipeX = screenWidth;
            pipeHeight = rand() % 300 + 100;
            score++;
        }

       
        velocity += gravity;
        birdY += velocity;

        
        if (checkCollision()) {
            outtextxy(300, 300, "GAME OVER!");
            char buffer[20];
            sprintf(buffer, "Score: %d", score);
            outtextxy(300, 330, buffer);
            getch();
            break;
        }

       
        if (kbhit()) {
            char ch = getch();
            if (ch == 32) { 
                velocity = jump;
            }
        }

      
        char buffer[20];
        sprintf(buffer, "Score: %d", score);
        outtextxy(10, 10, buffer);

        delay(50);
    }

    closegraph();
    return 0;
}
