/* 
 * File:   main.c
 * Author: Wu Di
 *
 * Created on 27 May, 2020, 8:26 PM
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
    
    //Variables
    char msg[50];
    char msg2[50];
    char letter;
    int i = 0;
    signed short imu_data[7];
    unsigned char chardata[14];
    
   
    
    
    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk 
        
        //Heart beat LED
        heartBeat();
        imu_read(IMU_OUT_TEMP_L,imu_data,7);
        
        if (0){
            sprintf(msg,"g: %d %d %d ",imu_data[1],imu_data[2],imu_data[3]);
            drawString(0,0,msg,0);
            sprintf(msg,"a: %d %d %d ",imu_data[4],imu_data[5],imu_data[6]);
            drawString(0,8,msg,0);
            sprintf(msg,"t: %d ",imu_data[0]);
            drawString(0,16,msg,0);
        }
        else {
            bar_xy(imu_data[5],imu_data[4],msg);
        }
      
        

       
        ssd1306_update();
                
        
}

}
//x = -imu[4];
//y = imu[5]
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
//void bar_y(signed short y_accel){
//    
//}

