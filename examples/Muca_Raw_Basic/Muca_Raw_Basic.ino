#include <Muca.h>
//#define DEBUG

Muca muca;

void setup() {
  Serial.begin(115200);
#ifdef DEBUG
  Serial.println("Initializing...");
#endif
  muca.init(false);
#ifdef DEBUG
  Serial.print("Chip ID : ");
  Serial.println(muca.getRegister(0xA3));
  Serial.print("FW Library Version : ");
  Serial.println(muca.getRegister(0xA1) << 8 | muca.getRegister(0xA2));
  Serial.print("Firmware ID : ");
  Serial.println(muca.getRegister(0xA6));
#endif  
  //muca.printAllRegisters();
  muca.useRawData(true); // If you use the raw data mode, the interrupt will not work
  
  muca.setGain(31);
  
  // Custom panels :
  // Put a "0" when physical rx or tx line is not connected, "1" instead
  bool rx[]={1,1,1,1,1,1,1,0,0,0,0,0};
  bool tx[]={1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  muca.selectLines(rx, tx); // Comment this line to use the full panel
#ifdef DEBUG
  Serial.print("Num_TX/Rows : ");
  Serial.println(muca.num_TX);
  Serial.print("Num_RX/Columns: ");
  Serial.println(muca.num_RX);
  // muca.grid[] is now num_TX * num_RX in size
#endif
  while(!Serial.available()) {  // Sync
    Serial.print("RX:");Serial.print(muca.num_RX);Serial.print(":TX:");Serial.print(muca.num_TX);Serial.println(":SYNC");
    delay(500);
  }
  Serial.read();
}

void loop() {
  if (muca.update()) {
    for (int i = 0; i < muca.num_TX * muca.num_RX; i++) {
      Serial.print(muca.grid[i]);
      if (i != muca.num_TX * muca.num_RX - 1)
        Serial.print(",");
    }
    Serial.println();
    while(!Serial.available());    // Sync
    Serial.read();
    //if(muca.num_RX*muca.num_TX < 150) {
    //  delay(10);    // Needed if panel size is small
    //}
  }
}
