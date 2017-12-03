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
byte pos1,pos2,pos3,pos3r,pos4,pos5,pos5x,pos5y,pos5z,pos6;
const byte d=200;//TO CONFIRM BLUETOOTH DATA NOT LOST and in correct order
boolean state=0;//for switching between gesture and remote control, default is remote and red LED ON
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(11, OUTPUT); //green LED Gesture Control
  pinMode(12, OUTPUT); //red LED Remote Control
  pos2=60;
  adxl.powerOn();                     // Power on the ADXL345
  Serial1.begin(9600);
  adxl.setRangeSetting(4);
 
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

void loop() {
  // put your main code here, to run repeatedly:
  char proc;
  // check if data has been sent from the computer:
  if (Serial.available()) 
  {
    // read the most recent byte (which will be from 0 to 255):
    proc = Serial.read();  //if multiple orders at the same time can be sent then fix this
  }
    if (proc == 't')//t //joystick button 11
    {
      state=!state;//toggle control methods
    }
    if(state==0)
    {
      digitalWrite(12,HIGH); //Red LED ON Remote control used
      digitalWrite(11,LOW);
      if (proc =='f') // f //RT
      {
        if(pos3<180)
        {
          pos3=pos3+1; //extend elbow
        }
      }
      else if(proc=='r') // r // LT
      {
        if(pos3>0)
        {
          pos3=pos3-1;//retract elbow
        }
      }
      else if (proc =='d') //d // L stick RIGHT
      {
        if(pos1<180)
        {
          pos1=pos1+1; //rotate base right
        }
      }
      else if(proc=='a')//a // L stick LEFT
      {
        if(pos1>0)
        {
          pos1=pos1-1;// rotate base left
        }
      }
      else if (proc =='w')//w L stick UP
      {
        if(pos2<180)
        {
          pos2=pos2+1;// base link up
        }
      }
      else if(proc=='s')//s L stick DOWN
      {
        if(pos2>45)
        {
          pos2=pos2-1;// base link down, minimum is 45 degrees 
        }
      }
      else if (proc =='j')//j // R stick RIGHT
      {
        if(pos4<180)
        {
          pos4=pos4+1; //Rotate wrist right
        }
      }
      else if(proc=='g')//g //R stick LEFT
      {
        if(pos4>0)
        {
          pos4=pos4-1;// Rotate wrist left
        }
      }
      else if (proc =='y')//y // R stick UP
      {
        if(pos5<180)
        {
          pos5=pos5+1;//Wrist up
        }
      }
      else if(proc=='h')//h // R stick DOWN
      {
        if(pos5>0)
        {
          pos5=pos5-1; // Wrist down 
        }
      }
      else if (proc =='q')//q //Button1
      {
        if(pos6>0)
        {
          pos6=pos6-1;//claw open
        }
      }
      else if(proc=='e')//e //Button 2
      {
        if(pos6<40)
        {
          pos6=pos6+1;//claw close
        }
      }
    }
    else
    {
      digitalWrite(12,LOW);//Green LED ON and Gesture part Used
      digitalWrite(11,HIGH);
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
  //  millisnow=millis();
    //if(millisnow-prevmillis>500)
   // {
      heading=heading+(millis()/9000);//heading drift correction drifts 1 degree down in 8 seconds when static
      h=(int)heading;
      //Serial.println("Corrected");
     // prevmillis=millisnow;
    //}
    
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
    pos2=constrain(pos2,60,180);//minimum angle shoulder can go in roboarm is this
    //map(p,90,270,0,180);  //+-something to get appropiate range
    pos3=map(p,-40,40,180,0);
        pos3=constrain(pos3,0,180);
    pos3r=pos3-pos2;
    pos3r=constrain(pos3r,0,180);
   // pos1=h;//+-something to get appropiate range
    pos1=map(h,140,220,180,0);
    pos1=constrain(pos1,0,180);
    r=r+90;//+-something to get appropiate range
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
    if(r>135)
    {
      pos4=180;
    }
    else if(r>45 && r<135 )
    {
      pos4=90;
    }
    else
    { 
      pos4=0;
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
    }
    Serial1.print(d);
  Serial1.print(",");
  Serial1.print(pos1);
  Serial1.print(",");
  Serial1.print(pos2);
  Serial1.print(",");
  Serial1.print(pos3);
  Serial1.print(",");
  Serial1.print(pos4);
  Serial1.print(",");
  Serial1.print(pos5);
  Serial1.print(",");
  Serial1.println(pos6);
  Serial1.flush();
  delay(100);//the rate at which we send is faster than what the receiver reads becoz performing servo functions too, so a delay is added here to reduce how 
             //frequently we send the data, it simply piles up in the bluetooth serial and gives the lag we were seeeing. probably have to change this when gestue 
             // code is added maybe won't even need it
  
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
//this code gives no lag yet
