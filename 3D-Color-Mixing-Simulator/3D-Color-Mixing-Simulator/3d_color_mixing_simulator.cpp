#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include <algorithm>
using namespace std;

struct Cube {
    float x, y;
    int size;
    int depth;
    int color;
    float angle;
    bool dragging;
};

// Screen center
const int WIDTH = 640;
const int HEIGHT = 480;

// Simulated light direction for shading
float lightDirX = -1.0f;
float lightDirY = -1.0f;

// Mix colors as in your original code
int mixColor(int c1,int c2){
    if((c1==RED && c2==GREEN)||(c1==GREEN && c2==RED)) return YELLOW;
    if((c1==BLUE && c2==GREEN)||(c1==GREEN && c2==BLUE)) return CYAN;
    if((c1==RED && c2==BLUE)||(c1==BLUE && c2==RED)) return MAGENTA;
    return WHITE;
}

void getRGB(int color, int &r,int &g,int &b){
    switch(color){
        case RED: r=255; g=0; b=0; break;
        case GREEN: r=0; g=255; b=0; break;
        case BLUE: r=0; g=0; b=255; break;
        case YELLOW: r=255; g=255; b=0; break;
        case CYAN: r=0; g=255; b=255; break;
        case MAGENTA: r=255; g=0; b=255; break;
        default: r=255; g=255; b=255; break;
    }
}

// Check if mouse is over cube (2D bounding box)
bool isMouseOver(Cube c,int mx,int my){
    return (mx>=c.x-c.size/2 && mx<=c.x+c.size/2 &&
            my>=c.y-c.size/2 && my<=c.y+c.size/2);
}

// Rotate a 2D point around a center
void rotatePoint(float cx, float cy, float &x, float &y, float angle){
    float s = sin(angle);
    float c = cos(angle);
    float dx = x-cx;
    float dy = y-cy;
    x = cx + dx*c - dy*s;
    y = cy + dx*s + dy*c;
}

// Simulated shading factor based on face orientation
float faceShading(float nx, float ny) {
    // normalize
    float len = sqrt(nx*nx + ny*ny + 1e-5f);
    nx /= len; ny /= len;
    float dot = nx*lightDirX + ny*lightDirY;
    return 0.5f + 0.5f*dot; // 0.0-1.0
}

// Draw cube with pseudo 3D, simulated Gouraud shading
void drawCube(Cube c){
    float half = c.size/2.0f;

    float frontX[4] = {c.x-half, c.x+half, c.x+half, c.x-half};
    float frontY[4] = {c.y-half, c.y-half, c.y+half, c.y+half};

    float topX[4] = {frontX[0], frontX[0]+c.depth, frontX[1]+c.depth, frontX[1]};
    float topY[4] = {frontY[0], frontY[0]-c.depth, frontY[1]-c.depth, frontY[1]};

    float sideX[4] = {frontX[1], frontX[1]+c.depth, frontX[2]+c.depth, frontX[2]};
    float sideY[4] = {frontY[1], frontY[1]-c.depth, frontY[2]-c.depth, frontY[2]};

    for(int i=0;i<4;i++){
        rotatePoint(c.x,c.y,frontX[i],frontY[i],c.angle);
        rotatePoint(c.x,c.y,topX[i],topY[i],c.angle);
        rotatePoint(c.x,c.y,sideX[i],sideY[i],c.angle);
    }

    int r,g,b;
    getRGB(c.color,r,g,b);

    // Apply shading factors
    int frontColor = COLOR(r*faceShading(0,0.0f), g*faceShading(0,0.0f), b*faceShading(0,0.0f));
    int topColor   = COLOR(r*faceShading(0,-1.0f), g*faceShading(0,-1.0f), b*faceShading(0,-1.0f));
    int sideColor  = COLOR(r*faceShading(1.0f,0), g*faceShading(1.0f,0), b*faceShading(1.0f,0));

    int poly[8];

    // Draw front face
    setfillstyle(SOLID_FILL, frontColor);
    poly[0]=frontX[0]; poly[1]=frontY[0];
    poly[2]=frontX[1]; poly[3]=frontY[1];
    poly[4]=frontX[2]; poly[5]=frontY[2];
    poly[6]=frontX[3]; poly[7]=frontY[3];
    fillpoly(4,poly);

    // Draw top face
    setfillstyle(SOLID_FILL, topColor);
    poly[0]=topX[0]; poly[1]=topY[0];
    poly[2]=topX[1]; poly[3]=topY[1];
    poly[4]=topX[2]; poly[5]=topY[2];
    poly[6]=topX[3]; poly[7]=topY[3];
    fillpoly(4,poly);

    // Draw side face
    setfillstyle(SOLID_FILL, sideColor);
    poly[0]=sideX[0]; poly[1]=sideY[0];
    poly[2]=sideX[1]; poly[3]=sideY[1];
    poly[4]=sideX[2]; poly[5]=sideY[2];
    poly[6]=sideX[3]; poly[7]=sideY[3];
    fillpoly(4,poly);
}

// Compute overlap cube as in your original code
bool getOverlap(Cube a, Cube b, Cube &overlap){
    int x1=max(a.x-a.size/2,b.x-b.size/2);
    int y1=max(a.y-a.size/2,b.y-b.size/2);
    int x2=min(a.x+a.size/2,b.x+b.size/2);
    int y2=min(a.y+a.size/2,b.y+b.size/2);

    if(x1<x2 && y1<y2){
        overlap.x=(x1+x2)/2.0f;
        overlap.y=(y1+y2)/2.0f;
        overlap.size=min(x2-x1,y2-y1);
        overlap.depth=min(a.depth,b.depth);
        overlap.color=mixColor(a.color,b.color);
        overlap.angle=(a.angle+b.angle)/2.0f;
        return true;
    }
    return false;
}

int main(){
    int gd=DETECT, gm;
    initgraph(&gd,&gm,(char*)"");

    Cube red={150,250,80,30,RED,0,false};
    Cube green={300,250,80,30,GREEN,0,false};
    Cube blue={450,250,80,30,BLUE,0,false};
    Cube overlap;

    int page = 0;

    while(!kbhit()){
        setactivepage(page);
        cleardevice();

        int mx=mousex();
        int my=mousey();

        if(GetAsyncKeyState(VK_LBUTTON)){
            if(isMouseOver(red,mx,my)) red.dragging=true;
            else if(isMouseOver(green,mx,my)) green.dragging=true;
            else if(isMouseOver(blue,mx,my)) blue.dragging=true;
        } else {
            red.dragging=green.dragging=blue.dragging=false;
        }

        if(red.dragging){ red.x=mx; red.y=my; }
        if(green.dragging){ green.x=mx; green.y=my; }
        if(blue.dragging){ blue.x=mx; blue.y=my; }

        red.angle += 0.03f;
        green.angle += 0.02f;
        blue.angle += 0.025f;

        // Draw cubes
        drawCube(red);
        drawCube(green);
        drawCube(blue);

        // Draw overlaps
        if(getOverlap(red,green,overlap)) drawCube(overlap);
        if(getOverlap(red,blue,overlap)) drawCube(overlap);
        if(getOverlap(green,blue,overlap)) drawCube(overlap);

        setvisualpage(page);
        page = 1-page;

        delay(16); // ~60 FPS
    }

    closegraph();
    return 0;
}
