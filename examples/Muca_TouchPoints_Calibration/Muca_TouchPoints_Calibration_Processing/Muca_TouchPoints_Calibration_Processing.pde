import processing.serial.*;
import controlP5.*;


Serial myPort;  // Create object from Serial class
char val;      // Data received from the serial port



// This register describes valid touching detect threshold.
int touchdetectthresh = 20; // 70*4 // The actual value will be 4 times of the register’s value. Default: 120/4

// This register describes valid touching peak detect threshold.
int touchpeak = 60; // Default: 60

// This register describes threshold when calculating the focus of touching.
int threshfocus = 16; // Default: 16

//This register describes threshold whether the coordinate is different from the original.
int threashdiff = 160; //The actual value must be 16 times of the register’s value. Default : 128

ControlP5 cp5;

int x = 900;
int y = 700;


void settings() {
    size(x, y);

}

void setup() 
{


  cp5 = new ControlP5(this);

  cp5.addSlider("touchdetectthresh")
    .setPosition(50, 50)
    .setRange(0, 80) ;

  cp5.addSlider("touchpeak")
    .setPosition(50, 70)
    .setRange(0, 150) ;

  cp5.addSlider("threshfocus")
    .setPosition(50, 90)
    .setRange(0, 40) ;

  cp5.addSlider("threashdiff")
    .setPosition(50, 110)
    .setRange(0, 255) ;

  cp5.addButton("send")
    .setValue(0)
    .setPosition(50, 130)
    .setSize(100, 19)
    ;


  cp5.addButton("calib")
    .setValue(0)
    .setPosition(50, 160)
    .setSize(45, 19)
    ;
      cp5.addButton("info")
    .setValue(0)
    .setPosition(105, 160)
    .setSize(45, 19)
    ;


  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 115200);
  
  set
}


// function colorA will receive changes from 
// controller with name colorA
public void send() {
  String t= touchdetectthresh + ":"+ touchpeak + ":" +threshfocus+":"+threashdiff+"\n";
  println("Sending: " + t);
  myPort.write(t);

}

public void calib() {
  String t= "a\n";
  println("Sending: " + t);
  myPort.write(t);
}
public void info() {
  String t= "i\n";
  println("Sending: " + t);
  myPort.write(t);
}

void draw()
{
  background(153);


readSerial();


}




void readSerial() {
  while ( myPort.available( ) > 0 ) {
    String data = myPort.readStringUntil( '\n' );
    if ( data != null ) {
      print(data);
    }
  }
}
