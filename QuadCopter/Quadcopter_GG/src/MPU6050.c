#include <stdio.h>
#include "MPU6050.h"
#define FCY     40000000UL
#include <math.h>
#include "common.h"
#include "I2C.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "bsp.h"

#define gyro_xsensitivity 66.5 //66.5 Dead on at last check
#define gyro_ysensitivity 66.5 //72.7 Dead on at last check
#define gyro_zsensitivity 65.5
#define a 0.01



void Setup_MPU6050()
{
	I2C_Setup();

	//Sets sample rate to 1000/1+1 = 500Hz
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, 0x01);

	//Disable FSync, 48Hz DLPF
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_CONFIG, 0x03);
	//Disable gyro self tests, scale of 500 degrees/s
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, 0x04);
	//Disable accel self tests, scale of +-4g, no DHPF
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, 0x04);
	//Freefall threshold of <|0mg|
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FF_THR, 0x00);
	//Freefall duration limit of 0
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FF_DUR, 0x00);
	//Motion threshold of >0mg
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_MOT_THR, 0x00);
	//Motion duration of >0s
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, 0x00);
	//Zero motion threshold
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, 0x00);
	//Zero motion duration threshold
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, 0x00);
	//Disable sensor output to FIFO buffer
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FIFO_EN, 0x00);

	//AUX I2C setup
	//Sets AUX I2C to single master control, plus other config
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL, 0x00);
	//Setup AUX I2C slaves
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL, 0x00);
 	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI, 0x00);


 	//MPU6050_RA_I2C_MST_STATUS //Read-only
 	//Setup INT pin and AUX I2C pass through
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, 0x00);
	//Enable data ready interrupt
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, 0x00);

	//MPU6050_RA_DMP_INT_STATUS		//Read-only
	//MPU6050_RA_INT_STATUS 3A		//Read-only
    //MPU6050_RA_ACCEL_XOUT_H 		//Read-only
    //MPU6050_RA_ACCEL_XOUT_L 		//Read-only
    //MPU6050_RA_ACCEL_YOUT_H 		//Read-only
    //MPU6050_RA_ACCEL_YOUT_L 		//Read-only
    //MPU6050_RA_ACCEL_ZOUT_H 		//Read-only
    //MPU6050_RA_ACCEL_ZOUT_L 		//Read-only
    //MPU6050_RA_TEMP_OUT_H 		//Read-only
    //MPU6050_RA_TEMP_OUT_L 		//Read-only
    //MPU6050_RA_GYRO_XOUT_H 		//Read-only
    //MPU6050_RA_GYRO_XOUT_L 		//Read-only
    //MPU6050_RA_GYRO_YOUT_H 		//Read-only
    //MPU6050_RA_GYRO_YOUT_L 		//Read-only
    //MPU6050_RA_GYRO_ZOUT_H 		//Read-only
    //MPU6050_RA_GYRO_ZOUT_L 		//Read-only
    //MPU6050_RA_EXT_SENS_DATA_00 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_01 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_02 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_03 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_04 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_05 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_06 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_07 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_08 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_09 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_10 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_11 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_12 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_13 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_14 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_15 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_16 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_17 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_18 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_19 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_20 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_21 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_22 	//Read-only
    //MPU6050_RA_EXT_SENS_DATA_23 	//Read-only
    //MPU6050_RA_MOT_DETECT_STATUS 	//Read-only

	//Slave out, dont care
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO, 0x00);
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO, 0x00);
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO, 0x00);
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO, 0x00);
	//More slave config
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL, 0x00);
	//Reset sensor signal paths
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, 0x00);
	//Motion detection control
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL, 0x00);
	//Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, 0x00);
	//Sets clock source to gyro reference w/ PLL
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, 0b00000010);
	//Controls frequency of wakeups in accel low power mode plus the sensor standby modes
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, 0x00);
    //MPU6050_RA_BANK_SEL			//Not in datasheet
    //MPU6050_RA_MEM_START_ADDR		//Not in datasheet
    //MPU6050_RA_MEM_R_W			//Not in datasheet
    //MPU6050_RA_DMP_CFG_1			//Not in datasheet
    //MPU6050_RA_DMP_CFG_2			//Not in datasheet
    //MPU6050_RA_FIFO_COUNTH		//Read-only
    //MPU6050_RA_FIFO_COUNTL		//Read-only
	//Data transfer to and from the FIFO buffer
	I2C_RegisterSet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, 0x00);
    //MPU6050_RA_WHO_AM_I 			//Read-only, I2C address

	//printf("\f");
	//printf("\nMPU6050 Setup Complete");
}

void MPU6050_Test_I2C()
{
	uint8_t Data = 0x00;
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_WHO_AM_I, &Data, 1);

	if(Data == 0x68)
	{
		//printf("\nI2C Read Test Passed, MPU6050 Address: 0x%x", Data);
	}
	else
	{
		//printf("ERROR: I2C Read Test Failed, Stopping");
		while(1){}
	}
}
int MPU6050_Check_Registers()
{
	unsigned char Data = 0x00;
	unsigned char Failed = 0;

	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_SMPLRT_DIV, &Data, 1);
	if(Data != 0x01) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_CONFIG, &Data, 1);
	if(Data != 0x03) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_CONFIG, &Data, 1);
	if(Data != 0x04) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_CONFIG, &Data, 1);
	if(Data != 0x04) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FF_THR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FF_DUR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_MOT_THR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_MOT_DUR, &Data, 1);
	if(Data != 0x00) {  Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ZRMOT_THR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ZRMOT_DUR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FIFO_EN, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_MST_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_ADDR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_REG, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_ADDR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_REG, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_ADDR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_REG, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_ADDR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_REG, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_ADDR, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_REG, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DO, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
 	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV4_DI, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_INT_PIN_CFG, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_INT_ENABLE, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV0_DO, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV1_DO, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV2_DO, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_SLV3_DO, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_I2C_MST_DELAY_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_SIGNAL_PATH_RESET, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_MOT_DETECT_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_USER_CTRL, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_1, &Data, 1);
	if(Data != 0x02) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_PWR_MGMT_2, &Data, 1);
	if(Data != 0x00) { Failed = 1; }
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_FIFO_R_W, &Data, 1);
	if(Data != 0x00) { Failed = 1; }

	return(Failed);
}

void Calibrate_Gyros()
{
	int x = 0;
	GYRO_XOUT_OFFSET_1000SUM = 0;
	GYRO_YOUT_OFFSET_1000SUM = 0;
	GYRO_ZOUT_OFFSET_1000SUM = 0;
	for(x = 0; x<5000; x++)
	{
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, &GYRO_XOUT_H, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L, &GYRO_XOUT_L, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, &GYRO_YOUT_H, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L, &GYRO_YOUT_L, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, &GYRO_ZOUT_H, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L, &GYRO_ZOUT_L, 1);

		GYRO_XOUT_OFFSET_1000SUM += ((GYRO_XOUT_H<<8)|GYRO_XOUT_L);
		GYRO_YOUT_OFFSET_1000SUM += ((GYRO_YOUT_H<<8)|GYRO_YOUT_L);
		GYRO_ZOUT_OFFSET_1000SUM += ((GYRO_ZOUT_H<<8)|GYRO_ZOUT_L);

		for(int i = 0; i < 14000; i++);
	}
	GYRO_XOUT_OFFSET = (int16_t)(GYRO_XOUT_OFFSET_1000SUM/5000);
	GYRO_YOUT_OFFSET = (int16_t)(GYRO_YOUT_OFFSET_1000SUM/5000);
	GYRO_ZOUT_OFFSET = (int16_t)(GYRO_ZOUT_OFFSET_1000SUM/5000);

	//printf("\nGyro X offset sum: %ld Gyro X offset: %d", (long)GYRO_XOUT_OFFSET_1000SUM, GYRO_XOUT_OFFSET);
	//printf("\nGyro Y offset sum: %ld Gyro Y offset: %d", (long)GYRO_YOUT_OFFSET_1000SUM, GYRO_YOUT_OFFSET);
	//printf("\nGyro Z offset sum: %ld Gyro Z offset: %d", (long)GYRO_ZOUT_OFFSET_1000SUM, GYRO_ZOUT_OFFSET);
}

void Calibrate_Acc()
{
	int x = 0;
	ACC_XOUT_OFFSET_1000SUM = 0;
	ACC_YOUT_OFFSET_1000SUM = 0;
	ACC_ZOUT_OFFSET_1000SUM = 0;
	for(x = 0; x<5000; x++)
	{
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, &ACCEL_XOUT_H, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_L, &ACCEL_XOUT_L, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H, &ACCEL_YOUT_H, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_L, &ACCEL_YOUT_L, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H, &ACCEL_ZOUT_H, 1);
		I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_L, &ACCEL_ZOUT_L, 1);

		ACC_XOUT_OFFSET_1000SUM += ((ACCEL_XOUT_H<<8)|ACCEL_XOUT_L);
		ACC_YOUT_OFFSET_1000SUM += ((ACCEL_YOUT_H<<8)|ACCEL_YOUT_L);
		ACC_ZOUT_OFFSET_1000SUM += ((ACCEL_ZOUT_H<<8)|ACCEL_ZOUT_L);

		for(int i = 0; i < 14000; i++);
	}
	ACC_XOUT_OFFSET = (int16_t)(ACC_XOUT_OFFSET_1000SUM/5000);
	ACC_YOUT_OFFSET = (int16_t)(ACC_YOUT_OFFSET_1000SUM/5000);
	ACC_ZOUT_OFFSET = (int16_t)(ACC_ZOUT_OFFSET_1000SUM/5000);

	//printf("\nGyro X offset sum: %ld Gyro X offset: %d", (long)ACC_XOUT_OFFSET_1000SUM, ACC_XOUT_OFFSET);
	//printf("\nGyro Y offset sum: %ld Gyro Y offset: %d", (long)ACC_YOUT_OFFSET_1000SUM, ACC_YOUT_OFFSET);
	//printf("\nGyro Z offset sum: %ld Gyro Z offset: %d", (long)ACC_ZOUT_OFFSET_1000SUM, ACC_ZOUT_OFFSET);
}

//Gets raw accelerometer data, performs no processing
void Get_Accel_Values()
{
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_H, &ACCEL_XOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_XOUT_L, &ACCEL_XOUT_L, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_H, &ACCEL_YOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_YOUT_L, &ACCEL_YOUT_L, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_H, &ACCEL_ZOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_ACCEL_ZOUT_L, &ACCEL_ZOUT_L, 1);

	ACCEL_XOUT = ((ACCEL_XOUT_H<<8)|ACCEL_XOUT_L) - ACC_XOUT_OFFSET;
	ACCEL_YOUT = ((ACCEL_YOUT_H<<8)|ACCEL_YOUT_L) - ACC_YOUT_OFFSET;
	ACCEL_ZOUT = ((ACCEL_ZOUT_H<<8)|ACCEL_ZOUT_L) - ACC_ZOUT_OFFSET + 8192;
//
//	printf("Accelerometer X %d\n", (short)ACCEL_XOUT);
//	printf("Accelerometer Y %d\n", (short)ACCEL_YOUT);
//	printf("Accelerometer Z %d\n\n", (short)ACCEL_ZOUT);
}

//Converts the already acquired accelerometer data into 3D euler angles
void Get_Accel_Angles()
{
	//ACCEL_XANGLE = 57.295*atan((float)ACCEL_YOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_XOUT,2)))*a + (1-a)*ACCEL_XANGLE;
	//ACCEL_YANGLE = 57.295*atan((float)-ACCEL_XOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_YOUT,2)))*a + (1-a)*ACCEL_YANGLE;

	ACCEL_XANGLE = 57.295*atan((float)ACCEL_YOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_XOUT,2)));
	ACCEL_YANGLE = 57.295*atan((float)-ACCEL_XOUT/ sqrt(pow((float)ACCEL_ZOUT,2)+pow((float)ACCEL_YOUT,2)));
}

//Function to read the gyroscope rate data and convert it into degrees/s
void Get_Gyro_Rates()
{
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, &GYRO_XOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L, &GYRO_XOUT_L, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, &GYRO_YOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L, &GYRO_YOUT_L, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, &GYRO_ZOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L, &GYRO_ZOUT_L, 1);

	GYRO_XOUT = ((GYRO_XOUT_H<<8)|GYRO_XOUT_L); //- GYRO_XOUT_OFFSET;
	GYRO_YOUT = ((GYRO_YOUT_H<<8)|GYRO_YOUT_L) - GYRO_YOUT_OFFSET;
	GYRO_ZOUT = ((GYRO_ZOUT_H<<8)|GYRO_ZOUT_L) - GYRO_ZOUT_OFFSET;


	GYRO_XRATE = (float)GYRO_XOUT/gyro_xsensitivity;
	GYRO_YRATE = (float)GYRO_YOUT/gyro_ysensitivity;
	GYRO_ZRATE = (float)GYRO_ZOUT/gyro_zsensitivity;

	GYRO_XANGLE += GYRO_XRATE*dt;
	GYRO_YANGLE += GYRO_YRATE*dt;
	GYRO_ZANGLE += GYRO_ZRATE*dt;

//	printf("Gyro X %d\n", GYRO_XOUT);
//	printf("Gyro Y %d\n", GYRO_YOUT);
//	printf("Gyro Z %d\n\n", GYRO_ZOUT);
//
//	printf("Gyro X angle %f\n", GYRO_XANGLE);
//	printf("Gyro Y angle %f\n", GYRO_YANGLE);
//	printf("Gyro Z angle %f\n", GYRO_ZANGLE);
}

/*void Get_Gyro_Raw_Rates()
{
	signed int GYRO_XOUT = 0;
	signed int GYRO_YOUT = 0;
	signed int GYRO_ZOUT = 0;


	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_H, &GYRO_XOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_XOUT_L, &GYRO_XOUT_L, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_H, &GYRO_YOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_YOUT_L, &GYRO_YOUT_L, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_H, &GYRO_ZOUT_H, 1);
	I2C_RegisterGet(I2C1, MPU6050_ADDRESS, MPU6050_RA_GYRO_ZOUT_L, &GYRO_ZOUT_L, 1);

	GYRO_XOUT = ((GYRO_XOUT_H<<8)|GYRO_XOUT_L) - GYRO_XOUT_OFFSET;
	GYRO_YOUT = ((GYRO_YOUT_H<<8)|GYRO_YOUT_L) - GYRO_YOUT_OFFSET;
	GYRO_ZOUT = ((GYRO_ZOUT_H<<8)|GYRO_ZOUT_L) - GYRO_ZOUT_OFFSET;

	GYRO_XRATERAW = GYRO_XOUT;
	GYRO_YRATERAW = GYRO_YOUT;
	GYRO_ZRATERAW = GYRO_ZOUT;
}*/
