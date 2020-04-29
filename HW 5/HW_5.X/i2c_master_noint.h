#ifndef I2C_MASTER_NOINT_H__
#define I2C_MASTER_NOINT_H__
// Header file for i2c_master_noint.c
// helps implement use I2C1 as a master without using interrupts

#include <xc.h>

#define IO_EXP_ADD_W 0b01000000 //IO expander address when writing
#define IO_EXP_ADD_R 0b01000001 //IO expander address when reading

void i2c_master_setup(void); // set up I2C1 as master
void i2c_master_start(void); // send a START signal
void i2c_master_restart(void); // send a RESTART signal
void i2c_master_send(unsigned char byte); // send a byte (either an address or data)
unsigned char i2c_master_recv(void); // receive a byte of data
void i2c_master_ack(int val); // send an ACK (0) or NACK (1)
void i2c_master_stop(void); // send a stop
void i2c_write(unsigned char address, unsigned char Register, unsigned char value);//writes to slave device
void i2c_pinsInit(void); //Initializes pins 
void i2c_setPin(unsigned char Register, unsigned char value);//Sets a pin value, pin must initialized as output
unsigned char i2c_readPin(unsigned char address, unsigned char Register); //Reads a pin value 
#endif