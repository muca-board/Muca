#include <Muca.h>
#define DEBUG

Muca muca;

#define CALIBRATION_STEPS 20
short currentCalibrationStep = 0;
unsigned int calibrationGrid[NUM_TX * NUM_RX];

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
  
  muca.setGain(8);
  
  // Custom panels :
  // Put a "0" when physical rx or tx line is not connected, "1" instead
  bool rx[]={1,1,1,1,1,1,1,1,1,1,0,0};
  bool tx[]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
  muca.selectLines(rx, tx); // Comment this line to use the full panel
#ifdef DEBUG
  Serial.print("Num_TX/Rows : ");
  Serial.println(muca.num_TX);
  Serial.print("Num_RX/Columns: ");
  Serial.println(muca.num_RX);
  // muca.grid[] is now num_TX * num_RX in size
#endif
  /*while(!Serial.available()) {  // Sync
    Serial.print("RX:");Serial.print(muca.num_RX);Serial.print(":TX:");Serial.print(muca.num_TX);Serial.println(":SYNC");
    delay(500);
  }*/
  //Serial.read();
  // Init calibration matrix
  for(int i=0; i < muca.num_TX * muca.num_RX; i++) {
    calibrationGrid[i] = 0;
  }
  
}

void loop() {
  GetRawCalib();
}

void GetRawCalib() {
  if (muca.update()) {
    for (int i= 0; i < muca.num_TX * muca.num_RX; i++) {
      if( abs(muca.grid[i] - calibrationGrid[i]) > 255 ) {
        calibrationGrid[i] = muca.grid[i];
      }
      
      //Serial.print((muca.grid[i]>65500?0:muca.grid[i]));
      Serial.print(muca.grid[i]-calibrationGrid[i]);
      if (i != muca.num_TX * muca.num_RX - 1)
        Serial.print(",");
    }
    Serial.println();
    //while(!Serial.available());    // Sync
    //Serial.read();
    //if(muca.num_RX*muca.num_TX < 150) {
      //delay(100);    // Needed if panel size is small
    //}
  }
}
void GetRaw() {
  if (muca.update()) {
    for (int i= 0; i < muca.num_TX * muca.num_RX; i++) {
      Serial.print((muca.grid[i]>65500?0:muca.grid[i]));
      //Serial.print(muca.grid[i]);
      if (i != muca.num_TX * muca.num_RX - 1)
        Serial.print(",");
    }
    Serial.println();
    //while(!Serial.available());    // Sync
    //Serial.read();
    //if(muca.num_RX*muca.num_TX < 150) {
      //delay(100);    // Needed if panel size is small
    //}
  }
}

char incomingMsg[20];

void serialEvent() {
  int charsRead;
  while (Serial.available() > 0) {
    charsRead = Serial.readBytesUntil('\n', incomingMsg, sizeof(incomingMsg) - 1);
    incomingMsg[charsRead] = '\0';  // Make it a string
    if (incomingMsg[0] == 'g') {
      Gain();
    }
   else if (incomingMsg[0] == 'c') {
      currentCalibrationStep = 0;
    }
  }
}


void Gain() {
  Serial.print("Received:"); Serial.println(incomingMsg);
  char *str;
  char *p = incomingMsg;
  byte i = 0;
  while ((str = strtok_r(p, ":", &p)) != NULL)  // Don't use \n here it fails
  {
    if (i == 1 )  {
      muca.setGain(atoi(str));
    }
    i++;
  }
  incomingMsg[0] = '\0'; // Clear array
}
