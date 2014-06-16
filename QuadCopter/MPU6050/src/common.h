	#define dt 0.0025

	extern short ACCEL_XOUT;
	extern short ACCEL_YOUT;
	extern short ACCEL_ZOUT;
	
	extern unsigned char GYRO_XOUT_L;
	extern unsigned char GYRO_XOUT_H;
	extern unsigned char GYRO_YOUT_L;
	extern unsigned char GYRO_YOUT_H;
	extern unsigned char GYRO_ZOUT_L;
	extern unsigned char GYRO_ZOUT_H;
	extern short GYRO_XOUT;
	extern short GYRO_YOUT;
	extern short GYRO_ZOUT;
	
	extern unsigned char ACCEL_XOUT_L;
	extern unsigned char ACCEL_XOUT_H;
	extern unsigned char ACCEL_YOUT_L;
	extern unsigned char ACCEL_YOUT_H;
	extern unsigned char ACCEL_ZOUT_L;
	extern unsigned char ACCEL_ZOUT_H;
	
	extern double GYRO_XOUT_OFFSET_1000SUM;
	extern double GYRO_YOUT_OFFSET_1000SUM;
	extern double GYRO_ZOUT_OFFSET_1000SUM;
	
	extern double ACC_XOUT_OFFSET_1000SUM;
	extern double ACC_YOUT_OFFSET_1000SUM;
	extern double ACC_ZOUT_OFFSET_1000SUM;
	
	extern short GYRO_XOUT_OFFSET;
	extern short GYRO_YOUT_OFFSET;
	extern short GYRO_ZOUT_OFFSET;
	
	extern short ACC_XOUT_OFFSET;
	extern short ACC_YOUT_OFFSET;
	extern short ACC_ZOUT_OFFSET;

	extern float PID_XOUTPUT;
	extern float PID_YOUTPUT;
	extern float PID_ZOUTPUT;
	
	extern float throttle;
	
	extern float KP;
	extern float KD;
	extern float KI;
	extern float ZKP;
	extern float ZKD;
	
	extern float XERROR;
	extern float YERROR;
	extern float ZERROR;
	
	extern float OC1_output;
	extern float OC2_output;
	extern float OC3_output;
	extern float OC4_output;
	
	extern float XINTEGRAL;
	extern float YINTEGRAL;

	extern int throttle_input;
	extern int yaw_input;
	extern int pitch_input;
	extern int roll_input;
