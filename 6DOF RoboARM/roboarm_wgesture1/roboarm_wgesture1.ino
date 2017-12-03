//INSTRUCTIONS TO CALIBRATE AT THE END
//make sure the head pitch roll hpr readings from imu dont drift, if they do make adjustments or code an alternative solution, if not then design your own hpr output code or drop the gesture
//in 3d(only linear) or gesture completely(only gamepad)
//Can add IR or flex or something for claw even LDR
//can make it wireless using bluetooth
//can use phone in wrist and acc and 101 and light sensor or proximity for claw 
//introduce byte lastval variables to reduce vibrations
//can write an initial calibration code
#include "CurieIMU.h"
#include <MadgwickAHRS.h>
#include <Servo.h>
#include <SparkFun_ADXL345.h> 
ADXL345 adxl = ADXL345();
Madgwick filter;
unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;
int a1raw,a2rawx,a2rawy,a2rawz,h,p,r; // accelero/sensor reading from arm, forearm, 101 on wrist remember to check positive readings from hpr
boolean claw=0; // sensor reading from button in hand
byte pos1,pos2,pos3,pos4,pos5,pos5x,pos5y,pos5z,pos6; // position variable for servos starting from base to claw; require only 0 to 180 values
byte pos3r;
Servo s1,s21,s22,s3,s4,s5,s6; //6 servos from base for 6 DOF also base has 2 servos so common signal pin for them
unsigned long prevmillis=0;
unsigned long millisnow=0;
void setup() 
{
  adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(4);
  s1.attach(2); 
  s21.attach(3);//left base motor
  s22.attach(8);//right base motor
  s3.attach(4);
  s4.attach(5);
  s5.attach(6);
  s6.attach(7);
  pinMode(10, INPUT_PULLUP); //digital pin for claw control
  CurieIMU.begin();
  CurieIMU.setGyroRate(25);
  CurieIMU.setAccelerometerRate(25);
  filter.begin(25);

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
  // Set the gyroscope range to 250 degrees/second
  CurieIMU.setGyroRange(250);

  // initialize variables to pace updates to correct rate
  microsPerReading = 1000000 / 25;
  microsPrevious = micros();
  // put your setup code here, to run once:
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  unsigned long microsNow;

  // check if it's time to read data and update the filter
  microsNow = micros();
  if (microsNow - microsPrevious >= microsPerReading) {

    // read raw data from CurieIMU
    CurieIMU.readMotionSensor(aix, aiy, aiz, gix, giy, giz);

    // convert from raw data to gravity and degrees/second units
    ax = convertRawAcceleration(aix);
    ay = convertRawAcceleration(aiy);
    az = convertRawAcceleration(aiz);
    gx = convertRawGyro(gix);
    gy = convertRawGyro(giy);
    gz = convertRawGyro(giz);

    // update the filter, which computes orientation
    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    millisnow=millis();
    if(millisnow-prevmillis>1000)
    {
      heading=heading+millis()/8000;//heading drift correction drifts 1 degree down in 8 seconds when static
      prevmillis=millisnow;
    }
    h=(int)heading;
    p=(int)pitch;
    r=(int)roll;
    adxl.readAccel(&a2rawx, &a2rawy, &a2rawz);         // Read the accelerometer values and store them in variables declared above x,y,z
  if(a2rawx>300)
  {
    a2rawx=a2rawx-65535;
  }
  if(a2rawy>300)
  {
    a2rawy=a2rawy-65535;
  }
  if(a2rawz>300)
  {
    a2rawz=a2rawz-65535;
  }
    a1raw = analogRead(A0);
    claw=digitalRead(10);
    //All sensor readings have been taken, now map it properply after processing
    pos2=map(a1raw,407,613,0,180);  
    pos2=constrain(pos2,45,180);//minimum angle shoulder can go in roboarm is this
    //map(p,90,270,0,180);  //+-something to get appropiate range
    pos3=map(p,-90,90,180,0);
    pos3r=pos3-pos2;
    pos3r=constrain(pos3r,0,180);
    pos1=h-90;//+-something to get appropiate range
    pos1=constrain(pos1,0,180);
    //r=r;//+-something to get appropiate range
    pos5x=map(a2rawx,138,-128,0,180);
    pos5x=constrain(pos5x,0,180);
    pos5y=map(a2rawy,-139,128,0,180);
    pos5y=constrain(pos5y,0,180);
    pos5z=map(a2rawz,-137,137,0,180);
    pos5z=constrain(pos5z,0,180);
    if (claw==1)
    {
      pos6=0;//close
    }
    else 
    {
      pos6=180; //open
    }
    s1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);
    s21.write(pos2);              // tell servo to go to position in variable 'pos'
    delay(15);
    s22.write(180-pos2);              // tell servo to go to position in variable 'pos'
    delay(15);
    s3.write(pos3r);              // tell servo to go to position in variable 'pos'
    delay(15);
    if(r>135)
    {
      pos4=180;
      s4.write(pos4);              // tell servo to go to position in variable 'pos'
      delay(15);
    }
    else if(r>45 && r<135 )
    {
      pos4=90;
      s4.write(pos4);              // tell servo to go to position in variable 'pos'
      delay(15);
    }
    else
    { 
      pos4=0;
      s4.write(pos4);              // tell servo to go to position in variable 'pos'
      delay(15);
    }
    if (pos4==90)
    {
      if (p<45)
      {
        pos5=map(pos5z-p,0,180,0,180);  
        pos5=constrain(pos5,0,180);
      }
      else// if(p>45)
      {
        pos5=map(pos5y-p,-90,90,0,180);  
        pos5=constrain(pos5,0,180);
      }
    }
    else
    {
      if (p<45)
      {
        pos5=map(pos5x-p,0,180,0,180);  
        pos5=constrain(pos5,0,180);
      }
      else// if(p>45)
      {
        pos5=map(pos5y-p,-90,90,0,180);  
        pos5=constrain(pos5,0,180);
      }
    }
    s5.write(pos5);              // tell servo to go to position in variable 'pos'
    delay(15);
    s6.write(pos6);              // tell servo to go to position in variable 'pos'
    delay(15);
}
}
float convertRawAcceleration(int aRaw) {
  // since we are using 2G range
  // -2g maps to a raw value of -32768
  // +2g maps to a raw value of 32767
  
  float a = (aRaw * 2.0) / 32768.0;
  return a;
}

float convertRawGyro(int gRaw) {
  // since we are using 250 degrees/seconds range
  // -250 maps to a raw value of -32768
  // +250 maps to a raw value of 32767
  
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
/*
 * //CALIBRATON
 * assuming mean position of arm and at that the hpr say 0 degrees and go from 0 to 360 degrees.
 * make everything in integer
 * place accelerometer in arm and wrist in correct orientation
 * place arduino 101 in correct orientation
 * refer hand written notes for more
 * 
 */
