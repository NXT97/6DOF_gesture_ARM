#include <Servo.h>
Servo s1,s2,s3,s4,s5,s6;//servo motors, actually base arm has 2 might add s22 and s21 if parallel doesn't work
byte pos1,pos2,pos3,pos4,pos5,pos6;
boolean state=0;//for switching between gesture and remote control, default is remote and red LED ON
void setup() {
  s1.attach(2); 
  s2.attach(3);
  s3.attach(4);
  s4.attach(5);
  s5.attach(6);
  s6.attach(7);
  // put your setup code here, to run once:
  Serial.begin(9600);
  pos2=45;
  pinMode(11, OUTPUT); //green LED Gesture Control
  pinMode(12, OUTPUT); //red LED Remote Control
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
        if(pos6<180)
        {
          pos6=pos6+1;//claw close
        }
      }
      s1.write(pos1);              // tell servo to go to position in variable 'pos'
      delay(15);
      s2.write(pos2);              // tell servo to go to position in variable 'pos'
      delay(15);
      s3.write(pos3);              // tell servo to go to position in variable 'pos'
      delay(15);
      s4.write(pos4);              // tell servo to go to position in variable 'pos'
      delay(15);
      s5.write(pos5);              // tell servo to go to position in variable 'pos'
      delay(15);
      s6.write(pos6);              // tell servo to go to position in variable 'pos'
      delay(15);
    }
    else
    {
      digitalWrite(12,LOW);//Green LED ON and Gesture part Used
      digitalWrite(11,HIGH);
    }
}
//check the serial data sending and receiving part add and integrate later
