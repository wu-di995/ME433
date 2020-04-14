/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _me433_H    /* Guard against multiple inclusion */
#define _me433_H

#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro

//User LED and Button
#define USERLED  LATAbits.LATA4
#define USERBTN  PORTBbits.RB4 //0 is pressed

//Time in core timer counts 
#define ONESEC_CC 24000000
#define TENMS_CC 240000
#define ONEMS_CC 24000

//me433 function prototypes
void me433_startup();
int debounce();
void blink(int cycles, float delay);
void delay_sec(float delay);
#endif
