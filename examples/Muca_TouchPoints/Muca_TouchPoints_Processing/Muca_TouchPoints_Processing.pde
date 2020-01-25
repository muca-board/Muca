import processing.serial.*;

// =========== CONSTANTS ==================
int     SKIN_COLS          = 12;
int     SKIN_ROWS          = 21;
int     SKIN_CELLS         = SKIN_COLS * SKIN_ROWS;

float   CELL_W             = 20;
float   CELL_H             = 20;

int     PHYSICAL_W         = 70; // mm
int     PHYSICAL_H         = 94; //mm

int     DISPLAY_W          = 700;
int     DISPLAY_H          = 700;

int     SERIAL_PORT        = 3; //32
int     SERIAL_RATE        = 115200;


// =========== PARSING ==================
char    SKIN_DATA_EOS      = '\n';
char    SKIN_DATA_SEP      = ',';

int     BLACK              = 0;
int     WHITE              = 255;


// =========== VARIABLES ==================
Serial  skinPort;
int[ ]  skinBuffer;
String  skinData      = null;
boolean skinDataValid = false;
PImage  skinImage     = createImage( SKIN_COLS, SKIN_ROWS, RGB );

boolean MIRROR_X = false; 
boolean MIRROR_Y = false;

int ROTATE =90; // 0, 90, 180, 270

void settings () { 
  size( DISPLAY_W, DISPLAY_H );
}

void setup () { 
  noStroke( );
  printArray(Serial.list());
  skinPort = new Serial( this, Serial.list( )[ SERIAL_PORT ], SERIAL_RATE );
  CELL_W = (PHYSICAL_W / SKIN_COLS) * 7;
  CELL_H = (PHYSICAL_H / SKIN_ROWS) * 7;
}


void draw() {
  readSkinBuffer( );
  background(200 );

  if ( skinDataValid ) {
    //drawSkinImage();
   
    pushMatrix();
     
     //MIRROR 
    // translate(MIRROR_X ? 0:CELL_W *SKIN_COLS, MIRROR_Y ? 0: CELL_H *SKIN_ROWS);
   // scale(MIRROR_X? 1:-1,MIRROR_Y ? 1:-1);
    scale(-1,1);
     
     
     //ROTATION
    // translate(ROTATE == 90? CELL_W *SKIN_COLS:0,  ROTATE == 270 ? CELL_H *SKIN_ROWS:0);
   //  rotate(radians(ROTATE));
     
     drawSkinHeatMap();
     
     popMatrix();
    
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

void drawSkinImage() {
  for ( int i = 0; i < SKIN_CELLS; i++ ) {
    int   X   = ( i % SKIN_COLS ) ;
    int   Y   = ( i / SKIN_COLS ) ;
    int colVal = computeColor(skinBuffer[i]); 
    skinImage.pixels[ i] = colVal;
  }
  skinImage.updatePixels( );
  noSmooth();
  image( skinImage, 0, 0);
  smooth(4);
}

void drawSkinHeatMap( ) {
  for ( int i = 0; i < SKIN_CELLS; i++ ) {
    int   X   = ( i % SKIN_COLS ) ;
    int   Y   = ( i / SKIN_COLS ) ;
    int colVal = computeColor(skinBuffer[i]); 
    fill(colVal);
    rect(X*CELL_W, Y*CELL_H, CELL_W, CELL_H);
  }
}

color computeColor( float value ) {
  float cons =   map(constrain(value, 10, 60), 10, 60, 0, 255);
  return color(cons, cons, cons);
}
