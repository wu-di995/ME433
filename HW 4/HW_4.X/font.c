#include <xc.h>
#include <stdio.h>
#include "font.h"
#include "ssd1306.h"
void drawChar(unsigned char x,unsigned char y, char Char){
    int col;
    int pixel;
    char color;
    unsigned char xpos =x;
    unsigned char ypos =y;
    for (col=0;col<=4;col++){
        for (pixel=0;pixel<=7;pixel++){
            color = ((ASCII[Char-0x20][col])>>pixel)&1;
            ssd1306_drawPixel(xpos,ypos,color);
            ypos +=1;
        }
        xpos+=1;
        ypos=y;
    }
    ssd1306_update();
    
};


void drawString(unsigned char x, unsigned char y, char* message, int showFPS){
    int i = 0;
    int j = 0;
    unsigned char xpos =x;
    float fps;
    char fps_array[15];
    unsigned char fps_x=70;
    unsigned char fps_y=25;
    
    unsigned char ypos =y;
    while (message[i]!=0){
        _CP0_SET_COUNT(0);
        j=0;
        drawChar(xpos,ypos,message[i]);
        if (showFPS==1){
            fps = (24000000.0/_CP0_GET_COUNT());
            sprintf(fps_array,"FPS:%.2f",fps);
            while (fps_array[j]!=0){
                drawChar(fps_x+j*5,fps_y,fps_array[j]);
                j++;
        }
        };
        i++;
        xpos = x+i*5;
        ypos = y;
    }
};

