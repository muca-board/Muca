import processing.serial.*;

Serial myPort;  // The serial port

int numRows = 21;
int numCols = 12 ;
boolean invertXY = true;

int[] rows;

int physicalHeight = 70; // mm
int physicalWidth = 94; //mm

boolean autoCalculateRect = true;
int rectWidth = 20;
int rectHeight = 20;

String myString = null;

void setup() {
  size(1000, 1000);

  //Open Serial Port
  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[4], 2000000);


  //Calculate settinss
  rectWidth = ( physicalWidth / numRows )  *5;
  rectHeight = (  physicalHeight / numCols ) *5;

  noStroke();
}

void draw() {

  boolean strOK = false;
  while (myPort.available() > 0) {
    myString = myPort.readStringUntil('\n');
    if (myString != null) {
      //     println(myString);
      rows = int(split(myString, ','));
      if (rows.length == numRows * numCols) strOK = true;
      else println("skip");
    }
  }



  if (strOK && rows.length == numRows * numCols) {
    background(150);
    GetFPS();
    for (int i =0; i < numRows * numCols; i++ ) {
      int x = i % numCols;    // % is the "modulo operator", the remainder of i / width;
      int y = i / numCols;    // where "/" is an integer division
      //   Debug.Log("x " + x + " y " + y);
      //println(rows[i]);
      fill(constrain(rows[i], 0, 255));
      rect(x*rectWidth, y*rectHeight, rectWidth, rectHeight);

      //  if (invertX) x = num_cols - x -1;
      // if (invertY) y = num_rows - y - 1;
      //  index = (num_cols * y) + x;
    }
  }


  /*
  for (int x = 0; x< numCols; x++) {
   for (int y = 0; y< numRow; y++) {
   
   
   }
   }
   */
}


int frameCount = 0;
float fps = 0.0F;
float t = 0.0F;
float prevtt = 0.0F;

void GetFPS()
{
  frameCount++;
  t += millis() - prevtt;
  if (t > 1000.0f)
  {
    fps = frameCount;
    frameCount = 0;
    t = 0;
  }
  prevtt = millis();
  text(fps, 500, 500);
}
