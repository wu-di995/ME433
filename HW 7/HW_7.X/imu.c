#include "imu.h"

void imu_setup(){
    unsigned char who = 0;
    
    // read from IMU_WHOAMI
    who = i2c_readPin(IMU_ADDR,IMU_WHOAMI);
    if (who != 0b01101001){
        while(1){}
    }
    // init IMU_CTRL1_XL
    i2c_write(IMU_ADDR,IMU_CTRL1_XL,0b10000000);
    // init IMU_CTRL2_G
    i2c_write(IMU_ADDR,IMU_CTRL2_G,0b10001000);
    // init IMU_CTRL3_C
    i2c_write(IMU_ADDR,IMU_CTRL3_C,0b00000100);
}

void imu_read(unsigned char reg, signed short * data, int len){
    
    // read multiple from the imu, each data takes 2 reads so you need len*2 chars
    unsigned char chardata[len*2];
    i2c_master_read_multiple(IMU_ADDR,reg,chardata,len*2);
    //convert to signed short array 
    int i;
    
    for (i=0;i<len;i++){
        data[i] = (chardata[2*i+1]<<8)|(chardata[2*i]);
    }
   
}