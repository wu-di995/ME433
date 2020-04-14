/* 
 * File:   me433.c
 * Author: Wu Di
 *
 * Created on 14 April, 2020, 12:16 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "me433.h"

void me433_startup(){
    //USER LED and Button 
    TRISAbits.TRISA4 = 0; //Configure A4 as output for User LED 
    TRISBbits.TRISB4 = 1; //Configure B4 as input for User button 
    USERLED = 0;
}

int debounce(){
    //User button pressed for at least 10ms 
    int press = 0;
    _CP0_SET_COUNT(0);
    while (USERBTN==0 && press< TENMS_CC){press=_CP0_GET_COUNT();} //increment press if still pressed 
    return press==TENMS_CC; //return 1 if press reaches TENMS_CC
}

void blink(int cycles, float delay){
    //cycles - no. of on/off cycles
    //delay - time in seconds in each on/off state
    int i =0;
        if (debounce()){
            while (i<cycles){
                USERLED = 1;
                delay_sec(0.5);
                USERLED = 0;
                delay_sec(0.5);
                i++;
            }
            i = 0;
        }
        else{
            USERLED = 0;
        } 
}

void delay_sec(float delay){
    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT()<(int)(delay*ONESEC_CC)){}
}

void delay_ms(float delay){
    _CP0_SET_COUNT(0);
    while(_CP0_GET_COUNT()<(int)(delay*ONEMS_CC)){}
}
