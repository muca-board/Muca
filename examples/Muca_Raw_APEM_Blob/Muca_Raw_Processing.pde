import processing.serial.*;

// =========== CONSTANTS ==================
int     SKIN_COLS          = 10;   // Max 12 (RX)
int     SKIN_ROWS          = 21;   // Max 21 (TX)
int     IMG_ROWS           = 11;   // SKIN_ROWS/2 
int     SKIN_CELLS         = SKIN_COLS * SKIN_ROWS;

int     SERIAL_PORT        = 0; 
int     SERIAL_RATE        = 115200;

boolean ROTATE             = false;
int     ZOOM               = 40;
int     val;

// =========== VARIABLES ==================
Serial  skinPort;
boolean skinDataValid = false;
boolean receiveDataAsByte = false;
boolean sync_OK = false;

int[ ]  skinBuffer, syncBuffer;
int[ ]  skinBufferCalibration = null;
PImage  skinImage;

// =========== DISPLAY ==================

int minThreshold = 50;
int maxThreshold = 120;  // == 1/gain

void settings () { 
  size( SKIN_COLS*ZOOM, IMG_ROWS*ZOOM );
  noSmooth();
}

void setup() { 
  noStroke();
  //colorMode(HSB, 1.0);
  printArray(Serial.list()); 
  skinPort = new Serial( this, Serial.list()[ SERIAL_PORT ], SERIAL_RATE );
  /*while (sync_OK == false) readSync();
  print("Size of panel read correctly");
  */
  //print("\nSKIN_COLS : ", SKIN_COLS, " SKIN_ROWS : ", SKIN_ROWS, "\n");
  //skinPort.write("s");
  
  skinImage = createImage( SKIN_COLS, IMG_ROWS, RGB );
}

void draw() {
  readSkinBuffer();
  background(200);
  if ( skinDataValid ) {
    IncrementFPS();
    //saveSkinImage();
  }
  //imageMode(CENTER);
  //image( skinImage, SKIN_COLS*ZOOM/2, SKIN_ROWS*ZOOM/2, SKIN_ROWS*ZOOM, SKIN_COLS*ZOOM);
  image( skinImage, 0, 0, SKIN_COLS*ZOOM, IMG_ROWS*ZOOM);
  fill(255);
  //text("Click to reset calibration Matrix", 10, 20);
  //text("FPS: " + fps, 10, 35);
  //val = skinImage.pixels[5*SKIN_COLS+3];// - #ff000000;
  //println(val);

  
  for(int i = 0; i<IMG_ROWS; i++) {
    for(int j = 0; j<SKIN_COLS; j++) {
      //text(skinImage.pixels[i*SKIN_COLS+j] & 0xFFFFFF, j*ZOOM + 5, i*ZOOM +15 );
      text(skinImage.pixels[i*SKIN_COLS+j] & 0xFF, j*ZOOM + 5, i*ZOOM +15 );
    }
  }

  if (mousePressed) {
    skinBufferCalibration = null;
  }
}

void readSkinBuffer() {
  if ( skinPort.available( ) > 0 ) {
      readDataAsString();
  } else {
    skinDataValid = false;
  }
}

char readSync(){
  String left = "";
  char test[];
  String syncData = skinPort.readStringUntil('\n');
  if ( syncData != null ) {
    print("length: ");
    for (int i=0; i<10; i++) {
      test = left;
      println(test);
    }
    if( syncData.equals(left) == true) {
      println("YEAH LLLL");
      return('l');
    }
    else {
                    println(syncData);
      if( syncData.equals("Right:\n")) {
        return('r');
     }
    }
  } 
  else { delay(10); // Let the serial port fill 
  }
  return('e');
}

void readDataAsString() {
  char side = readSync();
  if( side != 'e') {
    for( int i=0; i < 12; i++) {
      String skinData = skinPort.readStringUntil( '\n' );
      if ( skinData != null ) {
        if (skinData.length() > 10) {
            skinBuffer    = int( split( skinData, ',' ) );
            skinDataValid = skinBuffer.length == 5;
            switch(side) {
              case 'r':
              print('r');
              print(skinData);
                for( int j=0; j<5; j++) {
                  skinImage.pixels[i*10 +j+5]=skinBuffer[j];
                }
                break;
              case 'l':
              print('l');
              print(skinData);
                for( int j=0; j<5; j++) {
                  skinImage.pixels[i*10 +j]=skinBuffer[j];
                }
                break;
            }
        } else {
              print(skinData);
        }
      }
    }
  }
}

void saveSkinImage() {
  if (skinBufferCalibration == null) {
    skinBufferCalibration = new int[SKIN_CELLS];
    skinBufferCalibration = skinBuffer;
    println("Doing new calibration");
    println(skinBufferCalibration);
  }
  
  /*for ( int i = 0; i < SKIN_CELLS; i++ ) {
    int colVal = skinBuffer[i] - skinBufferCalibration[i];  // substract calibration matrix
    float cons =   map(constrain(colVal, minThreshold, maxThreshold), minThreshold, maxThreshold, 0, 255);
    color c = color(cons, cons, cons);
    skinImage.pixels[i] = c;
  }*/
  for(int i = 0; i<SKIN_ROWS; i++) {
    for(int j = 0; j<SKIN_COLS; j++) {
      int colVal = skinBuffer[i*SKIN_COLS+j] - skinBufferCalibration[i*SKIN_COLS+j];  // substract calibration matrix
      float cons =   map(constrain(colVal, minThreshold, maxThreshold), minThreshold, maxThreshold, 0, 255);
      color c = color(cons, cons, cons);
      //cons = colVal;
      // DEBUG : Override calibration value
      //c = skinBuffer[i*SKIN_COLS+j] & 0xFFFFFF;
      // No rotation (Demo board)
      if(i >= IMG_ROWS ) {  // lower half part of the image/sensor
        if(j >= 5) {
          skinImage.pixels[(i-IMG_ROWS)*SKIN_COLS + (SKIN_COLS-j-1)] = c;
        }
      } else {
        if(j < 5) {
          skinImage.pixels[(IMG_ROWS-i-1)*SKIN_COLS + (SKIN_COLS-j-1)] = c;
        }
      }
        // Rotate 180°
        //skinImage.pixels[(SKIN_COLS-i)*SKIN_COLS+(SKIN_ROWS-j)] = c;
      }
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
