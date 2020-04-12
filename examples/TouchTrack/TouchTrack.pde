import processing.serial.*;

Serial myPort;  // Create object from Serial class
char val;      // Data received from the serial port


// initialize a touch tracker with 5 touch points
TouchTracker touchTracker = new TouchTracker(5); 


void setup() {
  size(900, 700);

  String portName = Serial.list()[3];
  myPort = new Serial(this, portName, 115200);

  setResolution(900, 700);


  touchTracker.removeAfterDelay = true;
}


void draw()
{
  background(153);
  readSerial();

  for (int i = 0; i < touchTracker.touchCount(); i++) {
    touchTracker.GetTouch(i).draw();
  }

  touchTracker.update();
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

  String[] newTouchPoints = split(data, '|');

  for (int i =0; i < newTouchPoints.length; i++) {
    String[] touchData = split(newTouchPoints[i], ':');

    if (touchData.length == 5) {
      PVector pos =  new PVector( Integer.parseInt(touchData[2]), Integer.parseInt(touchData[3]));
      touchTracker.UpdateTouchPosition(pos, Integer.parseInt(touchData[4]));
    }
  }

  touchTracker.EndUpdate();
}

// send commands
public void setResolution(int w, int h) {
  String t= "r:" +w+":"+h+"\n";
  println("Sending: " + t);
  myPort.write(t);


  t= "g:10\n";
  println("Sending gain: " + t);
  myPort.write(t);
}
