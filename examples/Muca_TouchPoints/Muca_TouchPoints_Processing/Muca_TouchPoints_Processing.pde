import processing.serial.*;
import controlP5.*;



Serial myPort;  // Create object from Serial class
char val;      // Data received from the serial port




/*/////////////////////////////////////
 LIST SETTINGS
 
 10:30:9:97
 
 Clibration resets the settings
 
 
 *////////////////////////////////////


// This register describes valid touching detect threshold.
int touchdetectthresh = 20; // 70*4 // The actual value will be 4 times of the register’s value. Default: 120/4

// This register describes valid touching peak detect threshold.
int touchpeak = 60; // Default: 60

// This register describes threshold when calculating the focus of touching.
int threshfocus = 16; // Default: 16

//This register describes threshold whether the coordinate is different from the original.
int threashdiff = 160; //The actual value must be 16 times of the register’s value. Default : 128

//Control the difference value for touching 
int gain = 20; // 0 - 31



ControlP5 cp5;

int x = 900;
int y = 700;



TouchPoint touchPoints[] =  new TouchPoint[5];

void settings() {
  size(x, y);
}

void setup() 
{

  //TODO : add gain
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

  cp5.addSlider("gain")
    .setPosition(50, 130)
    .setRange(0, 31) ;


  cp5.addButton("send")
    .setValue(0)
    .setPosition(50, 150)
    .setSize(100, 19)
    ;


  cp5.addButton("calib")
    .setValue(0)
    .setPosition(50, 180)
    .setSize(45, 19)
    ;
  cp5.addButton("info")
    .setValue(0)
    .setPosition(105, 180)
    .setSize(45, 19)
    ;


  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 115200);

  setResolution(x, y);



  /////////// INIT TOUCH POINTS
  for (int i =0; i < touchPoints.length; i++) {
    touchPoints[i] = new TouchPoint(i);
  }
}

// function colorA will receive changes from 
// controller with name colorA
public void setResolution(int w, int h) {
  String t= "r:" +w+":"+h+"\n";
  println("Sending: " + t);
  myPort.write(t);
}
// function colorA will receive changes from 
// controller with name colorA
public void send() {
  String t= touchdetectthresh + ":"+ touchpeak + ":" +threshfocus+":"+threashdiff+"\n";
  println("Sending: " + t);
  myPort.write(t);

  t= "g:"+ gain+"\n";
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

  DrawTouchPoints();
}





void readSerial() {
  while ( myPort.available( ) > 0 ) {
    String data = myPort.readStringUntil( '\n' );
    if ( data != null ) {
      if (data.contains("|")) ParseTouchPoints(data);
    }
  }
}


// TODO : redo the system of finger tracking from strack based on the SkinInput COntroller

void ParseTouchPoints(String data) {
  println(data);

  // Parse New Data
  String[] newTouchPoints = split(data, '|');
  ArrayList<Integer> activeHardwareId = new ArrayList<Integer>();

  for (int i =0; i < newTouchPoints.length; i++) {
    String[] touchData = split(newTouchPoints[i], ':');

    if (touchData.length == 5) {
      int id = Integer.parseInt(touchData[0]);
      int x = Integer.parseInt(touchData[2]);
      int y = Integer.parseInt(touchData[3]);
      int w = Integer.parseInt(touchData[4]);
      touchPoints[id].Update(x, y, w);
      activeHardwareId.add(id);
    }
  }

println(activeHardwareId.size());

  // Remove not active points
  for (int i =0; i < touchPoints.length; i++) {
    boolean keepActive = false;
    if (touchPoints[i].isActive()) {
      for (int activeHard : activeHardwareId) {
        if(activeHard == i) {
          keepActive = true;
        }
      }
    }
    
    if(!keepActive) touchPoints[i].Remove();
  }
}

void DrawTouchPoints() {
  for (int i =0; i < touchPoints.length; i++) {
    touchPoints[i].Draw();
  }
}
