/* 
 * File:   main.c
 * Author: Wu Di
 *
 * Created on 28 April, 2020, 10:25 PM
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
    ws2812b_setup();
    
    //Variables
    char msg[50];
    char letter;
    int i = 0; int j = 0;
    
    //Set Color 
    wsColor colors[4];
    float sat = 1;
    float brightness = 0.02;
    float hue[4] = {0,30,60,90};
    
    //Addressable LEDs Testing
//        for (i=0;i<4;i++){
//            colors[i].r = 240;
//            colors[i].g = 120;
//            colors[i].b = 0;
//        }
   
  
    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk 
        
        //Heart beat LED
//        heartBeat();
//        float hue = 0; float hue2=30; float hue3=60; float hue4=90;
        
        //Method 1 
//        for (i=0;i<=200;i++){
//            for (j=0;j<4;j++){
//                hue = 180+180.0*sin(2*M_PI*i/200+(j*M_PI/6));
//                colors[j] = HSBtoRGB(hue,sat,brightness);
//            }
//            ws2812b_setColor(colors,4);
//            delay_sec(0.15);
//        }
        //Method 2
        for (j=0;j<4;j++){
            hue[j] = hue[j]+0.5;
//            sprintf(msg,"%f",hue[j]);
//            drawString(10,10,msg,0);
//            delay_sec(0.5);
            if (hue[j]>360){hue[j]=0.0;}
            colors[j] = HSBtoRGB(hue[j],sat,brightness);
            }
        ws2812b_setColor(colors,4);
        delay_sec(0.02);
        
        
        
        
//        for (j=0;j<4;j++){
//            hue = j*30;
//            for (i=0;i<=360;i++){
//                hue +=1;
//                colors[j] = HSBtoRGB(hue,sat,brightness);
//                ws2812b_setColor(colors,4);
//                delay_sec(0.25);
//            }
//        }

        
      
}

}