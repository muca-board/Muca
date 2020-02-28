import processing.serial.*;
import controlP5.*;


Serial myPort;  // Create object from Serial class
char val;      // Data received from the serial port



int peak = 70;
int cal = 60;
int thresh = 16;
int diff = 160;

ControlP5 cp5;



void setup() 
{
  size(640, 360);


  cp5 = new ControlP5(this);

  cp5.addSlider("peak")
    .setPosition(100, 50)
    .setRange(0, 80) ;

  cp5.addSlider("cal")
    .setPosition(100, 70)
    .setRange(0, 150) ;




  cp5.addSlider("thresh")
    .setPosition(100, 90)
    .setRange(0, 40) ;


  cp5.addSlider("diff")
    .setPosition(100, 110)
    .setRange(0, 255) ;


  cp5.addButton("send")
    .setValue(0)
    .setPosition(100, 130)
    .setSize(50, 19)
    ;

  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
}


// function colorA will receive changes from 
// controller with name colorA
public void send() {
  
  String t= peak + ":"+ cal + ":" +thresh+":"+diff+"\n";
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
