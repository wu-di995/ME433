// I2C Master utilities, using polling rather than interrupts
// The functions must be called in the correct order as per the I2C protocol
// I2C pins need pull-up resistors, 2k-10k
#include "i2c_master_noint.h"

void i2c_master_setup(void) {
    // using a large BRG to see it on the nScope, make it smaller after verifying that code works
    // look up TPGD in the datasheet
    I2C1BRG = 30; // I2CBRG = [1/(2*Fsck) - TPGD]*Pblck - 2 (TPGD is the Pulse Gobbler Delay)
    I2C1CONbits.ON = 1; // turn on the I2C1 module
}

void i2c_master_start(void) {
    I2C1CONbits.SEN = 1; // send the start bit
    while (I2C1CONbits.SEN) {
        ;
    } // wait for the start bit to be sent
}

void i2c_master_restart(void) {
    I2C1CONbits.RSEN = 1; // send a restart 
    while (I2C1CONbits.RSEN) {
        ;
    } // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
    I2C1TRN = byte; // if an address, bit 0 = 0 for write, 1 for read
    while (I2C1STATbits.TRSTAT) {
        ;
    } // wait for the transmission to finish
    if (I2C1STATbits.ACKSTAT) { // if this is high, slave has not acknowledged
        // ("I2C1 Master: failed to receive ACK\r\n");
        while(1){} // get stuck here if the chip does not ACK back
    }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C1CONbits.RCEN = 1; // start receiving data
    while (!I2C1STATbits.RBF) {
        ;
    } // wait to receive the data
    return I2C1RCV; // read and return the data
}

void i2c_master_ack(int val) { // sends ACK = 0 (slave should send another byte)
    // or NACK = 1 (no more bytes requested from slave)
    I2C1CONbits.ACKDT = val; // store ACK/NACK in ACKDT
    I2C1CONbits.ACKEN = 1; // send ACKDT
    while (I2C1CONbits.ACKEN) {
        ;
    } // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) { // send a STOP:
    I2C1CONbits.PEN = 1; // comm is complete and master relinquishes bus
    while (I2C1CONbits.PEN) {
        ;
    } // wait for STOP to complete
}

void i2c_write(unsigned char address, unsigned char Register, unsigned char value){
    //Start bit 
    i2c_master_start();
    //Address - 0b0100 A2 A1 A0 0, last 0 is the write bit, _ indicates address values
    //Send address
    i2c_master_send(address); //Write to slave device, for us A2=A1=A0 = 0
    //Send register
    i2c_master_send(Register);
    //Send value
    i2c_master_send(value);
    //Stop bit
    i2c_master_stop();
}

unsigned char i2c_readPin(unsigned char address, unsigned char Register){
    //Address input must have the write bit
    
    //Start bit 
    i2c_master_start();
    //Address - 0b0100 A2 A1 A0 0, last 0 is the write bit, _ indicates address values
    //Send address with write bit 
    i2c_master_send(address); //Write to slave device, for us A2=A1=A0 = 0
    //Send register
    i2c_master_send(Register);
    //Restart bit
    i2c_master_restart();
    //Send address with read bit 
    address = address | 0x01; //change the write bit to read bit 
    i2c_master_send(address);
    //Receive 
    unsigned char recv_value = i2c_master_recv();
    //Acknowledge 
    i2c_master_ack(1);
    //Stop bit
    i2c_master_stop();
    return recv_value;
}

void i2c_pinsInit(void){
    i2c_write(IO_EXP_ADD_W,0x00,0x00);//IODIRA, set all A pins to output
    i2c_write(IO_EXP_ADD_W,0x01,0xFF);//IODIRB, set all B pins to input
//    i2c_write(0b01000000,0x14,0xFF);//set all A pins to high
}

void i2c_setPin(unsigned char Register, unsigned char value){
    i2c_write(IO_EXP_ADD_W,Register,value);
}

void i2c_master_read_multiple(unsigned char address, unsigned char reg, unsigned char *data, int len){
     //Start bit 
    i2c_master_start();
    //Send address with write bit 
    i2c_master_send(address);
    //Send register
    i2c_master_send(reg);
    //Restart bit
    i2c_master_restart();
    //Send address with read bit 
    address = address | 0x01; //change the write bit to read bit 
    i2c_master_send(address);
    int i;
    for(i=0;i<len;i++){
        data[i] = i2c_master_recv();
        //Acknowledge 0 to continue reading
        if (i==len-1){
            i2c_master_ack(1);
        }
        else{
            i2c_master_ack(0);
        }   
    }
    //Stop bit
    i2c_master_stop();
}