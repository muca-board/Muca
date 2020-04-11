import processing.serial.*;

Serial myPort;  // Create object from Serial class
char val;      // Data received from the serial port





void setup() {
  size(900, 700);

  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 115200);

  setResolution(900, 700);
}


void draw()
{
  background(153);

  readSerial();
}







//////////////////////////////
//  Serial specific things
/////////////////////////////


void readSerial() {
  while ( myPort.available( ) > 0 ) {
    String data = myPort.readStringUntil( '\n' );
    if ( data != null ) {
      if (data.contains("|")) ParseTouchPoints(data);
    }
  }
}


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
      //  touchPoints[id].Update(x, y, w);
      activeHardwareId.add(id);
    }
  }
}


public void setResolution(int w, int h) {
  String t= "r:" +w+":"+h+"\n";
  println("Sending: " + t);
  myPort.write(t);
  
  
  t= "g:10\n";
  println("Sending gain: " + t);
  myPort.write(t);
}
