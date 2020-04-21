/* 
 * File:   spi.c
 * Author: Wu Di
 *
 * Created on 20 April, 2020, 9:52 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "spi.h"


// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn of analog pins
    ANSELA = 0; // 1 means analog 
    // Make A0 output pin for CS (chip-select)
    TRISAbits.TRISA0 = 0;//Configure A0 as output
    LATAbits.LATA0 = 1; //Turn CS high, CS is active low 
    //...
    // Set A1 pin for SDO1
    RPA1Rbits.RPA1R=0b0011;
    // Set B5 pin for SDI1
    SDI1Rbits.SDI1R = 0b0001;

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}

unsigned char spi_io(unsigned char o){
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF){;} //wait to receive the byte
    return SPI1BUF;
}

void writeA_halfmax(){
    unsigned short voltage = 2048; //short is 2 bytes
    unsigned char channel = 0;
    unsigned short command;
    command = channel<<15;
    command = command | (0b111<<12);
    command = command | voltage;
    spi_io(command>>8); //send the high bits
    spi_io(command); //send the low bits 
}

void CS_ON(){
    LATAbits.LATA0 = 0; //A0 is the CS pin, active low
}

void CS_OFF(){
    LATAbits.LATA0 = 1; //A0 is the CS pin, active low
}

//void sineWave(){
//    //2Hz
//    unsigned short voltage=0;
//    //Voltage = 2048*sin(2pi*freq*t)
//    // from max range 0 to 2048 -> quarter wave -> Period/4
//    // 1 increment per time step 
//    unsigned char channel = 0;
//    unsigned short command;
//    command = channel<<15;
//    command = command | (0b111<<12);
//    unsigned short i=0;
//    float timestep = 0.5/8192;
//    //2048x4 = 8192 timesteps 
//    for (i=0;i<4096;i++){
//        CS_ON();
//        voltage = i;
//        command = command | voltage;
//        spi_io(command>>8); //send the high bits
//        spi_io(command); //send the low bits 
//        CS_OFF();
//        _CP0_SET_COUNT(0);
//        while(_CP0_GET_COUNT()<(int)(0.5*ONESEC_CC)){}
//    }
//    
//    
//}
