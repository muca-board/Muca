import processing.serial.*;

// =========== CONSTANTS ==================
int     SKIN_COLS          = 12;
int     SKIN_ROWS          = 21;
int     SKIN_CELLS         = SKIN_COLS * SKIN_ROWS;

int     PHYSICAL_W         = 70; // mm
int     PHYSICAL_H         = 94; //mm

int     DISPLAY_W          = SKIN_COLS*30;
int     DISPLAY_H          = SKIN_ROWS*30;

int     SERIAL_PORT        = 0; //32
int     SERIAL_RATE        = 115200;


// =========== PARSING ==================
char    SKIN_DATA_EOS      = '\n';
char    SKIN_DATA_SEP      = ',';


// =========== VARIABLES ==================
Serial  skinPort;
int[ ]  skinBuffer;
String  skinData      = null;
boolean skinDataValid = false;


// =========== OPENCV ==================

PImage  skinImage     = createImage( SKIN_COLS, SKIN_ROWS, RGB );

private PImage destImg; 



int resizeFactor = 30;

// Computer vision settings
int imgageProcessing = 4; // 0 INTER_NEAREST // 1 INTER_LINEAR  // 2 INTER_CUBIC  3 // INTER_AREA  4 // INTER_LANCZOS4


// Blob detection settings
boolean enableBlobDetection = true;
boolean drawBlobCenter = true;
boolean drawBlobContour = true;
boolean enableThreshold = true;
float thresholdBlob = 0.8f;
int thresholdMin = 120;
int thresholdMax = 255;



void settings () { 
  size( DISPLAY_W, DISPLAY_H );
  noSmooth();
}

void setup () { 
  noStroke( );
  printArray(Serial.list());

  skinPort = new Serial( this, Serial.list( )[ SERIAL_PORT ], SERIAL_RATE );
  destImg = createImage(skinImage.width * resizeFactor, skinImage.height  * resizeFactor, RGB);
  //destImg = createImage(skinImage.width * resizeFactor * (skinImage.width / PHYSICAL_W), skinImage.height  * resizeFactor * (skinImage.height / PHYSICAL_H), RGB);


}


void draw() {
  readSkinBuffer( );
  background(200);
  if ( skinDataValid ) {
    saveSkinImage();
  }
}

void readSkinBuffer() {
  while ( skinPort.available( ) > 0 ) {
    skinData = skinPort.readStringUntil( SKIN_DATA_EOS );
    if ( skinData != null ) {
      skinBuffer    = int( split( skinData, SKIN_DATA_SEP ) );
      skinDataValid = skinBuffer.length == SKIN_CELLS;
    }
  }
}

void saveSkinImage() {
  for ( int i = 0; i < SKIN_CELLS; i++ ) {
    int colVal = computeColor(skinBuffer[i]); 
    skinImage.pixels[i] = colVal;
  }
  skinImage.updatePixels( );
  image( skinImage, 0, 0, SKIN_COLS*30, SKIN_ROWS*30);
}

color computeColor( float value ) {
  float cons =   map(constrain(value, 10, 60), 10, 60, 0, 255);
  return color(cons, cons, cons);
}
