//always check the serial port of arduino
//caps lock is off so letters are pressed in small only
//make sure that small window which opens is opened and selected so program runs
import processing.serial.*;
Serial port;
void setup() {
  size(256, 150);


  println("Available serial ports:");
  println(Serial.list());

  // Uses the first port in this list (number 0).  Change this to
  // select the port corresponding to your Arduino board.  The last
  // parameter (e.g. 9600) is the speed of the communication.  It
  // has to correspond to the value passed to Serial.begin() in your
  // Arduino sketch.
  //port = new Serial(this, Serial.list()[0], 9600);  

  // If you know the name of the port used by the Arduino board, you
  // can specify it directly like this.
  port = new Serial(this, "COM3", 9600);
}

void draw() {
  if (keyPressed && key == 'd') {  
    char msg = 'd';
    port.write(msg);
  }
  else if (keyPressed && key == 'a') {
    char msg = 'a';
    port.write(msg);
  }
  else if (keyPressed && key == 'w') {
    char msg = 'w';
    port.write(msg);
  }
  else if (keyPressed && key == 's') {
    char msg = 's';
    port.write(msg);
  }
  else if (keyPressed && key == 'f') {
    char msg = 'f';
    port.write(msg);
  }
  else if (keyPressed && key == 'r') {
    char msg = 'r';
    port.write(msg);
  }
  else if (keyPressed && key == 'y'){
    char msg = 'y';
    port.write(msg);
  }
  else  if (keyPressed && key == 'g') {  
    char msg = 'g';
    port.write(msg);
  }
  else if (keyPressed && key == 'h') {
    char msg = 'h';
    port.write(msg);
  }
  else if (keyPressed && key == 'j') {
    char msg = 'j';
    port.write(msg);
  }
  else if (keyPressed && key == 't') {
    char msg = 't';
    port.write(msg);
  }
  else if (keyPressed && key == 'q') {
    char msg ='q';
    port.write(msg);
  }
  else if (keyPressed && key == 'e') {
    char msg ='e';
    port.write(msg);
  }
  else {
    char msg ='n';
    port.write(msg);
  }
  delay(200);
  //port.write(",");
}