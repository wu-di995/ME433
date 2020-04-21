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

#ifndef _SPI_H    /* Guard against multiple inclusion */
#define _SPI_H

#include <xc.h>                     // processor SFR definitions
#include <sys/attribs.h>            // __ISR macro

//Time in core timer counts 
#define ONESEC_CC 24000000

//Function Protyptes
void initSPI();
void CS_ON();
void CS_OFF();
void writeA_halfmax();
void sineWave();
unsigned char spi_io(unsigned char o);
#endif