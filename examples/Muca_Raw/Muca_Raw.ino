#include <Muca.h>

Muca muca;

void setup() {
  Serial.begin(115200);

  muca.init(); // useInterrupt ne fonctionne pas bien
  muca.useRaw = true;
 // muca.setGain(100);
}

void loop() {
  GetRaw();
}

void GetRaw() {
  if (muca.updated()) {
  GetFPS();
   /*
   for (int i = 0; i < NUM_ROWS * NUM_COLUMNS; i++) {
      if (muca.grid[i] > 0) Serial.print(muca.grid[i]);
      if (i != NUM_ROWS * NUM_COLUMNS - 1)
        Serial.print(",");
    }
   Serial.println();
   */
  }
  
 // delay(1);
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
