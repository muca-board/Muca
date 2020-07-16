import processing.serial.*;

// =========== CONSTANTS ==================
int     SKIN_COLS          = 12;
int     SKIN_ROWS          = 21;
int     SKIN_CELLS         = SKIN_COLS * SKIN_ROWS;

int     SERIAL_PORT        = 3; 
int     SERIAL_RATE        = 115200;


// =========== VARIABLES ==================
Serial  skinPort;
boolean skinDataValid = false;
boolean receiveDataAsByte = true;

int[ ]  skinBuffer;
int[ ]  skinBufferCalibration = null;
PImage  skinImage     = createImage( SKIN_COLS, SKIN_ROWS, RGB );

// =========== DISPLAY ==================

int minThreshold = 0;
int maxThreshold = 200;



void settings () { 
  size( SKIN_COLS*30, SKIN_ROWS*30 );
  noSmooth();
}

void setup () { 
  noStroke( );
  printArray(Serial.list());
  
  skinPort = new Serial( this, Serial.list( )[ SERIAL_PORT ], SERIAL_RATE );

}



void draw() {
  readSkinBuffer( );
  background(200);
  if ( skinDataValid ) {
    IncrementFPS();
    saveSkinImage();
  }

  image( skinImage, 0, 0, SKIN_COLS*30, SKIN_ROWS*30);
  fill(255);
  text("Click to reset calibration Matrix", 10, 20);
  text("FPS: " + fps, 10, 35);

  if (mousePressed) {
    skinBufferCalibration = null;
  }
}





void readSkinBuffer() {
  if ( skinPort.available( ) > 0 ) {
    if (receiveDataAsByte)
      readDataAsByte();
    else
      readDataAsString();
  } else {
    skinDataValid = false;
  }
}




void readDataAsString() {
  String skinData = skinPort.readStringUntil( '\n' );
  if ( skinData != null ) {
    skinBuffer    = int( split( skinData, ',' ) );
    skinDataValid = skinBuffer.length == SKIN_CELLS;
  }
}


void readDataAsByte() {
  //Todo : There seem to be a bug of "black cell". Most likely due to the substraction of skinBufferCalibration. 
  byte[] inBuffer = new byte[SKIN_CELLS+2];
  int minimum = 0;

  inBuffer = skinPort.readBytes(SKIN_CELLS+2);
  //  skinPort.readBytes(inBuffer);
  if (inBuffer.length == SKIN_CELLS+2) {
    byte lowByteMinimum = inBuffer[1];
    byte highByteMinimum = inBuffer[0];
    minimum =  (parseInt(highByteMinimum) << 8) | parseInt(lowByteMinimum) ;

    skinBuffer = new int[SKIN_CELLS];
    for ( int i = 0; i < inBuffer.length-2; i++ ) {
      skinBuffer[i]    = minimum + parseInt(inBuffer[i+2]);
    }
    skinDataValid = skinBuffer.length == SKIN_CELLS;
  }
}


void saveSkinImage() {
  if (skinBufferCalibration == null) {
    println("Doing new calibration");
    skinBufferCalibration = new int[SKIN_CELLS];
    skinBufferCalibration = skinBuffer;
  }
  for ( int i = 0; i < SKIN_CELLS; i++ ) {
    int colVal = skinBuffer[i] - skinBufferCalibration[i];  // substract calibration matrix
    float cons =   map(constrain(colVal, minThreshold, maxThreshold), minThreshold, maxThreshold, 0, 255);
    color c = color(cons, cons, cons);
    skinImage.pixels[i] = c;
  }
  skinImage.updatePixels( );
}




int countFrame = 0;
float fps = 0.0F;
float t = 0.0F;
float prevtt = 0.0F;

void IncrementFPS()
{
  countFrame++;
  t += millis() - prevtt;
  if (t > 1000.0f)
  {
    fps = countFrame;
    countFrame = 0;
    t = 0;
  }
  prevtt = millis();
}
