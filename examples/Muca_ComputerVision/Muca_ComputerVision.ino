#include <Muca.h>

Muca muca;

void setup() {
  Serial.begin(115200);

  muca.init(); 
  muca.useRawData(true); // If you use the raw data, the interrupt is not working
}

void loop() {
  GetRaw();
}

void GetRaw() {
  if (muca.updated()) {
    
   for (int i = 0; i < NUM_TX * NUM_RX; i++) {
      if (muca.grid[i] > 0) Serial.print(muca.grid[i]);
      if (i != NUM_TX * NUM_RX - 1)
        Serial.print(",");
    }
   Serial.println();
  }
  
 delay(1);
}
