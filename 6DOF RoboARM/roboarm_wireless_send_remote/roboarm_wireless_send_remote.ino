byte pos1,pos2,pos3,pos4,pos5,pos6;
const byte d=200;//TO CONFIRM BLUETOOTH DATA NOT LOST and in correct order
boolean state=0;//for switching between gesture and remote control, default is remote and red LED ON
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(11, OUTPUT); //green LED Gesture Control
  pinMode(12, OUTPUT); //red LED Remote Control
  pos2=45;
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
//this code gives no lag yet
