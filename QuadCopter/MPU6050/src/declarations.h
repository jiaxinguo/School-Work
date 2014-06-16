short ACCEL_XOUT = 0;
short ACCEL_YOUT = 0;
short ACCEL_ZOUT = 0;

unsigned char GYRO_XOUT_L = ' ';
unsigned char GYRO_XOUT_H = ' ';
unsigned char GYRO_YOUT_L = ' ';
unsigned char GYRO_YOUT_H = ' ';
unsigned char GYRO_ZOUT_L = ' ';
unsigned char GYRO_ZOUT_H = ' ';
short GYRO_XOUT = 0;
short GYRO_YOUT = 0;
short GYRO_ZOUT = 0;

unsigned char ACCEL_XOUT_L = ' ';
unsigned char ACCEL_XOUT_H = ' ';
unsigned char ACCEL_YOUT_L = ' ';
unsigned char ACCEL_YOUT_H = ' ';
unsigned char ACCEL_ZOUT_L = ' ';
unsigned char ACCEL_ZOUT_H = ' ';

double GYRO_XOUT_OFFSET_1000SUM = 0;
double GYRO_YOUT_OFFSET_1000SUM = 0;
double GYRO_ZOUT_OFFSET_1000SUM = 0;

double ACC_XOUT_OFFSET_1000SUM = 0;
double ACC_YOUT_OFFSET_1000SUM = 0;
double ACC_ZOUT_OFFSET_1000SUM = 0;

short GYRO_XOUT_OFFSET = 0;
short GYRO_YOUT_OFFSET = 0;;
short GYRO_ZOUT_OFFSET = 0;

short ACC_XOUT_OFFSET = 0;
short ACC_YOUT_OFFSET = 0;
short ACC_ZOUT_OFFSET = 0;

float PID_XOUTPUT = 0;
float PID_YOUTPUT = 0;
float PID_ZOUTPUT = 0;

float KP = 26.0; //25 27/6/12
float KI = 200.0; //85 5/6/12
float KD = 7; //7 27/6/12

float ZKP = 40.0; //40 8/6/12
float ZKD = 25.0; //25 8/6/12

float XERROR = 0;
float YERROR = 0;
float ZERROR = 0;

float throttle = 0;

float OC1_output = 0.0;
float OC2_output = 0.0;
float OC3_output = 0.0;
float OC4_output = 0.0;

int count = 0;

float XINTEGRAL = 0;
float YINTEGRAL = 0;

int throttle_input = 0;
int yaw_input = 0;
int pitch_input = 0;
int roll_input = 0;
