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

int     DISPLAY_W          = 650;
int     DISPLAY_H          = 700;

int     SERIAL_PORT        = 3; //32
int     SERIAL_RATE        = 115200;


// =========== PARSING ==================
char    SKIN_DATA_EOS      = '\n';
char    SKIN_DATA_SEP      = ',';


// =========== VARIABLES ==================
Serial  skinPort;
int[ ]  skinBuffer = new int[SKIN_CELLS]; 
String  skinData      = null;
boolean skinDataValid = false;



// =========== FILTER ==================
int filter = 0;


//// Filter 1
float k = 0.3f;
float[ ]  filteredCol = new float[SKIN_CELLS];
float[ ]  prevCol = new float[SKIN_CELLS];





// =========== OPENCV ==================

private OpenCV opencv; // openCV object for interpolation
PImage  skinImage     = createImage( SKIN_COLS, SKIN_ROWS, RGB );

private BlobDetection blobDetection; // BlobDetection object for BlobDetection
private PImage destImg; 




// =========== Threshold settings ==================
boolean autoThreshold = false; //  Auto threshold is not working well when there are skipped lines
int thresholdMin = 15;
int thresholdMax = 70;
int gainValue = 20;


// Visual settings


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
  readSkinBuffer();
  background(200);
  if ( skinDataValid ) {
    treatSkinData();
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




boolean lowThresholdSet = false;

int highestThreshold = 50;
int prevHighestThreshold = 50; // TODO : no use of that
int highestThresholdThisFrame = 0;
int average = 0;

void treatSkinData() {


  if (autoThreshold) {

    average = 0;
    int averageCount = 0;
    highestThresholdThisFrame = 0;
    for ( int i = 0; i < SKIN_CELLS; i++ ) {

      if (skinBuffer[i] > highestThreshold) {
        if (skinBuffer[i] < highestThreshold *2) { // Be sure it's not a too big value
          highestThreshold = skinBuffer[i];
        }
      }
      if (skinBuffer[i] > highestThresholdThisFrame) {
        highestThresholdThisFrame = skinBuffer[i];
      }
      if ( skinBuffer[i] <  highestThreshold/2) { // Ensure it's only the BG by anaysing if it's 
        average += skinBuffer[i];
        averageCount ++;
      }
    }

    average = average / averageCount;
    thresholdMin = average + 5; // Adding 5 to the minimum value

    thresholdMax = highestThreshold-10; //int(lerp(thresholdMax, (highestThreshold + highestThresholdThisFrame) / 2.0, 0.1));
   
   // TODO : this is not working
  /*  if(highestThresholdThisFrame > average * 2.5 && highestThresholdThisFrame < thresholdMax*2 && highestThresholdThisFrame < thresholdMax && prevHighestThreshold >= highestThreshold )  {
    //  highestThreshold = round((highestThresholdThisFrame * ko) + (prevHighestThreshold * (1.0f -ko) ) ) ; 
     highestThreshold = int(lerp(highestThreshold, (highestThreshold + highestThresholdThisFrame) / 2.0, 0.001));
      println("lerp");
    } 
    prevHighestThreshold = highestThreshold;
    */
  
    cp5.getController("thresholdMin").setValue(thresholdMin);
    cp5.getController("thresholdMax").setValue(thresholdMax);
  }


  for ( int i = 0; i < SKIN_CELLS; i++ ) {
    //int   X   = ( i % SKIN_COLS ) ;
    //int   Y   = ( i / SKIN_COLS ) ;
    int colVal = computeColor(skinBuffer[i]); 

    switch(filter) {
    case 1:
      float rawCol = map(constrain(skinBuffer[i], thresholdMin, thresholdMax), thresholdMin, thresholdMax, 0, 255);
      filteredCol[i] = (rawCol * k) + (prevCol[i] * (1.0f - k) ) ;
      colVal = color(filteredCol[i], filteredCol[i], filteredCol[i]);
      prevCol[i] =  filteredCol[i];
      break;
    }

    skinImage.pixels[i] = colVal;
  }
  skinImage.updatePixels( );
  image( skinImage, 0, 0);
}

color computeColor( float value ) {
    if(value > thresholdMax+40) value = 0;
  float c = constrain(value, thresholdMin, thresholdMax);
  float cons =   map(c, thresholdMin, thresholdMax, 0, 255);
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
