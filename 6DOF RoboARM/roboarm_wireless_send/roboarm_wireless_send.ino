//Arduino 101 used
byte pos1,pos2,pos3,pos4,pos5,pos6;
const byte d=200;//TO CONFIRM BLUETOOTH DATA NOT LOST
void setup() {
  // put your setup code here, to run once:
Serial1.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
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
}
/*
 * CAN use this method to ensure proper communication
https://www.arduino.cc/en/Tutorial/SerialCallResponse
 * Basically will send a character 'a' in void setup and wait until we receive something then continue
*/
