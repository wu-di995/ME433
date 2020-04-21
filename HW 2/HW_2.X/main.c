/* 
 * File:   main.c
 * Author: Wu Di
 *
 * Created on 20 April, 2020, 9:48 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro
#include <math.h>


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

void sineWave();
void triangleWave();
void sendBothWaves();


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
    
    // Initialize SPI
    initSPI();
    
    
    __builtin_enable_interrupts();
    
    
    while (1) {
        // use _CP0_SET_COUNT(0) and _CP0_GET_COUNT() to test the PIC timing
        // remember the core timer runs at half the sysclk 
        sendBothWaves();
//        triangleWave();
           
}

}

void sendBothWaves(){
    unsigned short i = 0;
    for (i=0;i<1000;i++){
        unsigned short voltage0,voltage1; //short is 2 bytes
        unsigned char channel0 = 0, channel1 = 1;
        unsigned short command0, command1;
        //Sine Wave
        voltage0 = (unsigned short) 2048+2047*sin(2*M_PI*2*i/1000);
        //Triangle Wave
        if (i<=500){
            voltage1 = 8*i;
        }
        else{
            voltage1 = 8*(1000-i);
        }
        
        
//        Send command0
        CS_ON(); //turn on CS
        command0 = channel0<<15;
        command0 = command0 | (0b111<<12);
        command0 = command0 | voltage0;
        spi_io(command0>>8); //send the high bits
        spi_io(command0); //send the low bits
        CS_OFF();//turn off CS
        
        
        //Send command1
        CS_ON(); //turn on CS
        command1 = channel1<<15;
        command1 = command1 | (0b111<<12);
        command1 = command1 | voltage1;
        spi_io(command1>>8); //send the high bits
        spi_io(command1); //send the low bits
        CS_OFF();//turn off CS
       
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<(int)(0.001*24000000)){}
    }

}



void sineWave(){
    unsigned short i = 0;
    for (i=0;i<1000;i++){
        CS_ON(); //turn on CS
        unsigned short voltage; //short is 2 bytes
        unsigned char channel = 0;
        unsigned short command;
        voltage = (unsigned short) 2048+2047*sin(2*M_PI*4*i/1000);
        command = channel<<15;
        command = command | (0b111<<12);
        command = command | voltage;
        spi_io(command>>8); //send the high bits
        spi_io(command); //send the low bits 
        CS_OFF();//turn off CS

        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<(int)(0.001*24000000)){}
    }
}

void triangleWave(){
    unsigned short i = 0;
    for (i=0;i<1000;i++){
        CS_ON(); //turn on CS
        unsigned short voltage; //short is 2 bytes
        unsigned char channel = 1;
        unsigned short command;
        if (i<=500){
            voltage = 8*i;
        }
        else{
            voltage = 8*(1000-i);
        }
        command = channel<<15;
        command = command | (0b111<<12);
        command = command | voltage;
        spi_io(command>>8); //send the high bits
        spi_io(command); //send the low bits 
        CS_OFF();//turn off CS
        _CP0_SET_COUNT(0);
        while(_CP0_GET_COUNT()<(int)(0.001*24000000)){}
    }
}
