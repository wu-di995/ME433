/* 
 * File:   main.c
 * Author: Wu Di
 *
 * Created on 28 May, 2020, 2:19 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include <math.h>
#include "i2c_master_noint.h"
#include "me433.h"
#include "ssd1306.h"
#include "font.h"
#include "imu.h"
#include "ws2812b.h"


// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

//Function Prototypes
void bar_xy(signed short x_accel,signed short y_accel, unsigned char *msg);
void LEDcontrol(int data4, int data5);
int main() {
    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;
    
    
    __builtin_enable_interrupts();
    
    //Setups 
    me433_startup();
    i2c_master_setup();
    i2c_pinsInit();
    ssd1306_setup();
    imu_setup();
    ws2812b_setup();
    adc_setup();
    
    //Variables
    char msg[50];
    char msg2[50];
    char letter;
    int i = 0;
    signed short imu_data[7];
    unsigned char chardata[14];
    int data4;
    int data5;
    
    
    wsColor colors[2];
    float sat = 1;
    float brightness = 0.02;
    
    
    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk 
        
        //Heart beat LED
        heartBeat();
        
        //Read Capacitance 
        int ctmu_delay = 500;
        //500, 15
        data4 = ctmu_avgRead(4,ctmu_delay);
        data5 = ctmu_avgRead(5,ctmu_delay);
        sprintf(msg,"AN4: %d",data4);
        sprintf(msg2,"AN5: %d",data5);
        drawString(0,0,msg,0);
        drawString(0,8,msg2,0);
        
//        int avgd4=0;
//        int avgd5=0;
//        int len = 50;
//        for (i=0;i<len;i++){
//            avgd4 += ctmu_avgRead(4,500);
//            avgd5 += ctmu_avgRead(5,500);
//        
//        }
//        avgd4 = avgd4/len;
//        avgd5 = avgd5/len;
//        sprintf(msg,"AN4: %d",avgd4);
//        sprintf(msg2,"AN5: %d",avgd5);
//        drawString(0,0,msg,0);
//        drawString(0,8,msg2,0);
//        int delL, delR, i=0,len=50;
//        float posL, posR, pos;
//        delL = (float) (355-avgd4);
//        delR = (float) (355-avgd5);
//        posL = (float)(delL*100)/(delL+delR);
//        posR = (float)((1-delR)*100)/(delL+delR);
//        pos = (posL+posR)/2;
   
//        LED 
//        colors[0] = HSBtoRGB(0,sat,0);
//        colors[1] = HSBtoRGB(0,sat,brightness);
//        ws2812b_setColor(colors,2);
        LEDcontrol(data4,data5);
        //Update screen
        ssd1306_update();
                
        
}

}

void LEDcontrol(int data4, int data5){
    float hue1=0, hue2=0, hue3 = 30, bright1, bright2, bright3=0, sat=1;
    wsColor colors[3]; 
    int thr4 = 3800, thr5 = 3000, base4=4700, base5 = 4600;
    int delL, delR, i=0,len=50;
    float posL, posR, pos;
    char msg[10];
    if (data4>thr4 & data5>thr5){
        bright1 = 0;
        bright2 = 0;
        
    }
    if (data4<thr4){
        float posavg = 0;
        bright1 = 0.02;
        
        
    }
    if (data5<thr5){
        bright2 = 0.02;
//        delL = (float) (base4-data4);
//        delR = (float) (base5-data5);
//        posL = (float)(delL*100)/(delL+delR);
//        posR = (float)((1-delR)*100)/(delL+delR);
//        pos = (float) ((posL+posR)/2.0);
//        for (i=0;i<len;i++){
//            delL = 355-ctmu_read(4,500);
//            delR = 355-ctmu_read(5,500);
//            posL = (delL*100)/(delL+delR);
//            posR = ((1-delR)*100)/(delL+delR);
//            pos = (posL+posR)/2;
//            posavg+=pos;
//        }
//        posavg = posavg/len;
//        sprintf(msg,"delL: %d",delL);
//        drawString(0,0,msg,0);
//        sprintf(msg,"delR: %d",delR);
//        drawString(0,8,msg,0);
//        sprintf(msg,"pos: %f",pos);
//        drawString(0,16,msg,0);
//        sprintf(msg,"posR: %d",posR);
//        bright3 = abs(pos)/100+0.02;
//        if (pos<0){
//            bright3 = abs(pos)/20+0.02;
//        }
//        if (pos>0){
//            bright3 = abs(pos)/500+0.02;
//        }
    }
    
    
//    bright1=0;
//    bright2=0;
    
    colors[0] = HSBtoRGB(hue1,sat,bright1);
    colors[1] = HSBtoRGB(hue2,sat,bright2);
    colors[2] = HSBtoRGB(hue3,sat,bright3);
    ws2812b_setColor(colors,3);

}



void bar_xy(signed short x_accel,signed short y_accel, unsigned char *msg){
    ssd1306_clear();
    int x_norm,y_norm,i; 
    x_norm = (int) x_accel/(64*4);
    y_norm = (int) y_accel/(64*16);
    ssd1306_drawPixel(64,16,1);
    sprintf(msg,"%d",y_norm);
    drawString(0,0,msg,0);
    if (x_norm<0){
        x_norm = abs(x_norm);
        for (i=0;i<x_norm;i++){
        ssd1306_drawPixel(64+i,16,1);
        }
    }
    else{
        for (i=0;i<x_norm;i++){
        ssd1306_drawPixel(64-i,16,1);
        }
    }
    if (y_norm<0){
        y_norm = abs(y_norm);
        for (i=0;i<y_norm;i++){
        ssd1306_drawPixel(64,16-i,1);
        }
    }
    else{
        for (i=0;i<y_norm;i++){
        ssd1306_drawPixel(64,16+i,1);
        }
    }
}