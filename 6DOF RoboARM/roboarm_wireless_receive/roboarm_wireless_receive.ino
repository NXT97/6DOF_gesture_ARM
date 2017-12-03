//Leonardo used
#include <Servo.h>
Servo s1,s21,s22,s3,s4,s5,s6;//servo motors, actually base arm has 2 might add s22 and s21 if parallel doesn't work
byte pos1,pos2,pos3,pos4,pos5,pos6;
byte d=0;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);//pins 0 1 bluetooth
  Serial.begin(9600);
  s1.attach(2); //lets hope pwm not required
  s21.attach(3);
  s22.attach(8);
  s3.attach(4);
  s4.attach(5);
  s5.attach(6);
  s6.attach(7);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available())
  {
    while(d!=200)
    {
      d=Serial1.parseInt();
    }
    d=0;
    pos1=Serial1.parseInt();
    pos2=Serial1.parseInt();
    pos3=Serial1.parseInt();
    pos4=Serial1.parseInt();
    pos5=Serial1.parseInt();
    pos6=Serial1.parseInt();
    while(Serial1.available()>0)
    {
      char temp=Serial1.read();
    }
  }
  Serial.print("pos1 ");
  Serial.print(pos1);
  Serial.print(",");
  Serial.print(pos2);
  Serial.print(",");
  Serial.print(pos3);
  Serial.print(",");
  Serial.print(pos4);
  Serial.print(",");
  Serial.print(pos5);
  Serial.print(",");
  Serial.println(pos6);
  s1.write(pos1);              // tell servo to go to position in variable 'pos'
  delay(15);
  s21.write(pos2);              // tell servo to go to position in variable 'pos'
  delay(15);
  s22.write(180-pos2);              // tell servo to go to position in variable 'pos'
  delay(15);
  s3.write(pos3);              // tell servo to go to position in variable 'pos'
  delay(15);
  s4.write(pos4);              // tell servo to go to position in variable 'pos'
  delay(15);
  s5.write(pos5);              // tell servo to go to position in variable 'pos'
  delay(15);
  s6.write(pos6);              // tell servo to go to position in variable 'pos'
  delay(15);
  //delay(100);
}
/*
 * CAN use this method to ensure proper communication
https://www.arduino.cc/en/Tutorial/SerialCallResponse
 * Basically will wait until we receive character 'a' and then will send 'a' or anything to acknowledge
*/
