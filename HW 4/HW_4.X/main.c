/* 
 * File:   main.c
 * Author: Wu Di
 *
 * Created on 28 April, 2020, 4:37 PM
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
    
    //Variables
    char msg[50];
    char letter;
    int i = 0;
   
    
    
    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk 
        
        //Heart beat LED
        heartBeat();
        unsigned char value = i2c_readPin(IO_EXP_ADD_W,0x13);
        if (i2c_readPin(IO_EXP_ADD_W,0x13)==0){//Read GPIOB
            i2c_setPin(0x14,0xFF); //Sets all A pins to high 
            delay_sec(0.5);
            i2c_setPin(0x14,0x00); //Sets all A pins to low
        }
        
        sprintf(&letter,"g");
        sprintf(msg,"My var = %d",i);
        
//        OLED Display
//        ssd1306_drawPixel(10,10,1);
//        ssd1306_update();
//        delay_sec(0.5);
//        ssd1306_drawPixel(10,10,0);
//        ssd1306_update();
        
//        drawChar(10,10,letter);
        drawString(5,5,msg,1);
        i++;
        if (i==1000){
            i = 0;
        }
//        showFPS(5,15);
        
        
}

}