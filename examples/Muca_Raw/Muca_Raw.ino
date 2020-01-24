#if (RAMEND < 1000)
  #define SERIAL_BUFFER_SIZE 16
#else
  #define SERIAL_BUFFER_SIZE 64
#endif

#include "MuCa_firmware.h"

MuCa muca;

void setup() {
  Serial.begin(115200);

  muca.init(); // useInterrupt ne fonctionne pas bien
  muca.useRaw = true;
 // muca.setGain(100);
}

void loop() {
  //GetTouch();
  GetRaw();
}

void GetRaw() {
  if (muca.updated()) {
 //  GetFPS();
   
   for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
      if (muca.grid[i] > 0) Serial.print(muca.grid[i]);
      if (i != NUM_ROWS * NUM_COLUMNS - 1)
        Serial.print(",");
    }
   Serial.println();
   
  }
  
 // delay(1);
}


void GetTouch() {
  if (muca.updated()) {
    for (int i = 0; i < muca.getNumberOfTouches(); i++) {
      Serial.print("Touch ");
      Serial.print(i);
      Serial.print("\tx:");
      Serial.print(muca.getTouch(i).x);
      Serial.print("\ty:");
      Serial.print(muca.getTouch(i).y);
      Serial.print("\tid:");
      Serial.print(muca.getTouch(i).id);
      Serial.print("\tweight:");
      Serial.print(muca.getTouch(i).weight);
      Serial.print(" |\t");
    }
    if ( muca.getNumberOfTouches() != 0) Serial.println("");
  }
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
  Serial.println(fps);
}
