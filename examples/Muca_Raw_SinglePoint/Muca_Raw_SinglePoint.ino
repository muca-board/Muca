#include <Muca.h>

Muca muca;

void setup() {
  Serial.begin(115200);

  muca.init();
  muca.useRawData(true);
  muca.setGain(8);
}

void loop() {
  for(int row = 1; row <=21; row++) { // NUM_ROWS 21
    for(int col = 1; col<= 12; col++) { // NUM_COLUMNS 12
      if(col+row !=2)
        Serial.print(",");
      int val = muca.getRawData(col,row);
       Serial.print(val);
   //     muca.getRawData(i,j);
    }
  }
  Serial.println();
 // Serial.println(muca.getRawData(5, 10)); // Get the point at the Column 5, Row 10 
  delay(100);
}
