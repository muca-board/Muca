import processing.serial.*;

import gab.opencv.*;
import org.opencv.imgproc.Imgproc;
import org.opencv.core.Mat;
import org.opencv.core.Size;
import org.opencv.core.CvType;
import org.opencv.core.Core;
import blobDetection.*;

// =========== CONSTANTS ==================
int     SKIN_COLS          = 12;
int     SKIN_ROWS          = 21;
int     SKIN_CELLS         = SKIN_COLS * SKIN_ROWS;

int     PHYSICAL_W         = 70; // mm
int     PHYSICAL_H         = 94; //mm

int     DISPLAY_W          = 700;
int     DISPLAY_H          = 700;

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

private OpenCV opencv; // openCV object for interpolation
PImage  skinImage     = createImage( SKIN_COLS, SKIN_ROWS, RGB );

private BlobDetection blobDetection; // BlobDetection object for BlobDetection
private PImage destImg; 




// Serial cettings
int thresholdMin = 0;
int thresholdMax = 20;



int resizeFactor = 30;

// Computer vision settings
int imgageProcessing = 0; // 0 INTER_NEAREST // 1 INTER_LINEAR  // 2 INTER_CUBIC  3 // INTER_AREA  4 // INTER_LANCZOS4


// Blob detection settings
boolean enableBlobDetection = false;
boolean drawBlobCenter = true;
boolean drawBlobContour = true;
boolean enableThreshold = false;
float thresholdBlob = 0.8f;
int thresholdBlobMin = 120;
int thresholdBlobMax = 255;



void settings () { 
  size( DISPLAY_W, DISPLAY_H );
  noSmooth();
}

void setup () { 
  noStroke( );
  printArray(Serial.list());
  opencv = new OpenCV(this, SKIN_COLS, SKIN_ROWS);
  skinPort = new Serial( this, Serial.list( )[ SERIAL_PORT ], SERIAL_RATE );
  destImg = createImage(skinImage.width * resizeFactor, skinImage.height  * resizeFactor, RGB);
  //destImg = createImage(skinImage.width * resizeFactor * (skinImage.width / PHYSICAL_W), skinImage.height  * resizeFactor * (skinImage.height / PHYSICAL_H), RGB);

  blobDetection = new BlobDetection(destImg.width, destImg.height);
  blobDetection.setThreshold(thresholdBlob);

  InterfaceSetup();
}


void draw() {
  readSkinBuffer( );
  background(200);
  if ( skinDataValid ) {
    saveSkinImage();
    performCV();

    pushMatrix();
    translate(30, 30);
    drawCV();
    if (enableBlobDetection) drawBlobs();
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

void saveSkinImage() {
  for ( int i = 0; i < SKIN_CELLS; i++ ) {
    //int   X   = ( i % SKIN_COLS ) ;
    //int   Y   = ( i / SKIN_COLS ) ;
    int colVal = computeColor(skinBuffer[i]); 
    skinImage.pixels[i] = colVal;
  }
  skinImage.updatePixels( );
  image( skinImage, 0, 0);
}

color computeColor( float value ) {
  float cons =   map(constrain(value, thresholdMin, thresholdMax), thresholdMin,thresholdMax, 0, 255);
  return color(cons, cons, cons);
}



void performCV() {
  opencv.loadImage(skinImage); // load image
  Mat skinImageBlackWhite = opencv.getGray(); // get grayscale matrix

  Mat skinImageRezied = new Mat(destImg.width, destImg.height, skinImageBlackWhite.type()); // new matrix to store resize image
  Size sz = new Size(destImg.width, destImg.height); // size to be resized

  // Imgproc.resize(skinImageBlackWhite, skinImageRezied, sz, 0, 0, Imgproc.INTER_CUBIC ); // resize // INTER_NEAREST // INTER_CUBIC  Imgproc.INTER_LANCZOS4
  Imgproc.resize(skinImageBlackWhite, skinImageRezied, sz, 0, 0, imgageProcessing); // resize // INTER_NEAREST

  if (enableThreshold) Imgproc.threshold(skinImageRezied, skinImageRezied, thresholdBlobMin, thresholdBlobMax, Imgproc.THRESH_BINARY);

  
  opencv.toPImage(skinImageRezied, destImg); // store in Pimage for drawing later

  if (enableBlobDetection) {
    blobDetection.computeBlobs(destImg.pixels);
    blobDetection.setThreshold(thresholdBlob);
  }
}


void drawCV() {
  // Draw the final image
  image(destImg, 0, 0);
}



public void drawBlobs() {
  Blob blob;
  EdgeVertex edgeA, edgeB;
  for (int n = 0; n < blobDetection.getBlobNb(); n++) {
    blob = blobDetection.getBlob(n);
    if (blob != null) {
      // Edges
      if (drawBlobContour) {
        strokeWeight(2);
        stroke(0, 255, 0);
        for (int m = 0; m < blob.getEdgeNb(); m++) {
          edgeA = blob.getEdgeVertexA(m);
          edgeB = blob.getEdgeVertexB(m);
          if (edgeA != null && edgeB != null)
            //   line(eA.x, eA.y, eB.x, eB.y);
            line(edgeA.x * destImg.width, edgeA.y * destImg.height, edgeB.x * destImg.width, edgeB.y * destImg.height); // when full width
          // line(eA.x * width, eA.y * height, eB.x * width, eB.y * height); // when full width
        }
        destImg.loadPixels();
      }

      // Blobs
      if (drawBlobCenter) {
        strokeWeight(5);
        point(blob.x * destImg.width, blob.y * destImg.height);
        loadPixels();
      }
    }
  }
}
