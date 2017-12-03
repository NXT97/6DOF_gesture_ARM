#include <CurieIMU.h>
#include <MadgwickAHRS.h>
#include <SparkFun_ADXL345.h> 
ADXL345 adxl = ADXL345();
Madgwick filter;

unsigned long microsPerReading, microsPrevious;
float accelScale, gyroScale;
unsigned long prevmillis=0;
unsigned long millisnow=0;
int h,p,r;
void setup() {
  Serial.begin(9600);
  while(!Serial);
adxl.powerOn();                     // Power on the ADXL345

  adxl.setRangeSetting(4);
  // start the IMU and filter
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
}

void loop() {
  int aix, aiy, aiz;
  int gix, giy, giz;
  float ax, ay, az;
  float gx, gy, gz;
  float roll, pitch, heading;
  int x,y,z;
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
      heading=heading+millis()/4000;//heading drift correction drifts 1 degree down in 8 seconds when static
      prevmillis=millisnow;
      //Serial.print("corrected")
    h=(int)heading;
    p=(int)pitch;
    r=(int)roll;
    Serial.print(h);
    Serial.print(" ");
    Serial.print(p);
    Serial.print(" ");
    Serial.print(r);
    Serial.print(" ");
     adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
  if(x>300)
  {
    x=x-65535;
  }
  if(y>300)
  {
    y=y-65535;
  }
  if(z>300)
  {
    z=z-65535;
  }
  /*Serial.print(x);
  Serial.print(", ");
  Serial.print(y);
  Serial.print(", ");
  Serial.print(z);
  Serial.print(", ");
  Serial.print(analogRead(A0));*/
  Serial.println();
  
    // increment previous time, so we keep proper pace
    microsPrevious = microsPrevious + microsPerReading;
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
