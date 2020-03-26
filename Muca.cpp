#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "Muca.h"


// https://github.com/sumotoy/FT5206/blob/master/FT5206.h <- info on registers


volatile bool newTouch = false;
void interruptmuca() {
  newTouch = true;
}


Muca::Muca() {}


byte Muca::readRegister(byte reg, short numberBytes) {

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(I2C_ADDRESS, numberBytes);
  byte readedValue = Wire.read();
  return readedValue;
  //Serial.print();
  /*while(Wire.available()) {
      Serial.print(Wire.read());
      Serial.print(" ");
    }*/
}

byte Muca::setRegister(byte reg, byte val) {

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(reg);
  Wire.write(val); 

  return Wire.endTransmission(false);;
}



void Muca::setConfig(byte touchdetectthresh, byte touchpeak, byte threshfocus, byte threashdiff ) {
  setRegister(0x80, touchdetectthresh);
  setRegister(0x81, touchpeak);
  setRegister(0x82, threshfocus);
  setRegister(0x85, threashdiff);

  setRegister(0xA0, 0x00); // enable auto calib

  setRegister(0x00,MODE_NORMAL); // DEVICE_MODE : NORMAL
}



void Muca::printAllRegisters() {

    setRegister(0x00, MODE_NORMAL); // ENsure test mode
   setRegister(0xA7, 0x03); // ID_G_ STATE   FACTORY

byte prev = 0;
  Serial.println(readRegister(0xc,1));

for(int i =0; i<=255;i++) {
  Serial.print(i,HEX);
  Serial.print("\t");
  byte current = readRegister(byte(i),1);
  Serial.print(current);
  Serial.print("\tTOTALPREV\t");
  unsigned int output = (prev << 8) | (current);
//unsigned int output = word(prev,current);
//Serial.print((current-1 << 8) | (prev));
//Serial.print("\t");

  Serial.println(output);
  prev = current;
}


}

void Muca::setResolution(unsigned short w, unsigned short h) {
width = w;
height = h;


/*
// TODO : not working cause c'est de la merde
delay(10);

Serial.print("width_high:");readRegister(0x9c,1);      Serial.print("\t");
Serial.print("width_low:");readRegister(0x9d,1);     Serial.print("\t");
Serial.print("height_high:");readRegister(0x9e,1);     Serial.print("\t");
Serial.print("height_low:");readRegister(0x9f,1);      Serial.print("\t");
  
  Serial.println();
*/
  /*
  delay(50);
  setRegister(0x00, MODE_NORMAL); // DEVICE_MODE : NORMAL
  delay(50);
  byte width_high = highByte(width);
  byte width_low = lowByte(width);
  byte height_high = highByte(height);
  byte height_low = lowByte(height);


  setRegister(0x98, width_high);
  setRegister(0x99, width_low);
  setRegister(0x9a, height_high);
  setRegister(0x9b, height_low);
  delay(10);
  setRegister(0x00,MODE_NORMAL); // DEVICE_MODE : NORMAL
  delay(10);
    Serial.println("[Muca] Set Resolution");
*/
}




void Muca::printInfo() {

  Serial.print("MODE\t");
  readRegister(0xA7, 1);
  Serial.print("\t");

  Serial.print("ID_G_THGROUP\t");
  readRegister(0x80, 1);
  Serial.print("\t");

  Serial.print("ID_G_THPEAK\t");
  readRegister(0x81, 1);
  Serial.print("\t");

  Serial.print("ID_G_THCAL\t");
  readRegister(0x82, 1);
  Serial.print("\t");

  Serial.print("ID_G_THDIFF\t");
  readRegister(0x85, 1);
  Serial.print("\t");

  Serial.print("AUTO_CLB_MODE\t");
  readRegister(0xA0, 1);
  Serial.print("\t");
  Serial.println();


  setRegister(0x00,MODE_NORMAL);


}


void Muca::autocal() {

  int error = 0;
  unsigned char i ;

  Serial.println("[FTS] start auto CLB.");
  delay(200);
  setRegister(0x00,MODE_TEST);
  delay(100);                       //make sure already enter factory mode

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(2));
  Wire.write(0x4); // fts_i2c_write_reg(client, 2, 0x4);
  // https://github.com/KonstaT/sailfishos_kernel_jolla_msm8930/blob/master/drivers/input/touchscreen/focaltech_ft5316_ts.c
  Wire.endTransmission(I2C_ADDRESS);


  delay(300);

  bool done = false;

  for (i = 0; i < 100; i++)
  {
    if (done) break;
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x00);
    //  Wire.write(0x40);
    Wire.endTransmission();
    //uint8_t
    Wire.requestFrom(I2C_ADDRESS, 1);

    byte reading = Wire.read();

    if ( ((reading & 0x70) >> 4) == 0x0)  //return to normal mode, calibration finish
    {
      done = true;
      Serial.println("[Muca] Calibration done!");
      break;
    }

    delay(200);
    Serial.println("[Muca] Waiting calibration...");
  }

  Serial.println("[Muca] Calibration OK.");

  delay(300);

  
  error = setRegister(0x00,MODE_TEST);

  if (error != 0) { Serial.print("[Muca] Calibration Error"); Serial.println(error);}

  delay(100);                       //make sure already enter factory mode



  error = setRegister(0x02,0x5); // SAVE CALIBRATION RESULT

  if (error != 0) {Serial.print("[Muca] Calibration Error"); Serial.println(error);}
  delay(300);

  setRegister(0x00,MODE_NORMAL); 


  delay(300);
  Serial.println("[Muca] Store CLB result OK.");
}

void Muca::init(bool raw = false) {
  useRaw = raw;
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);


  Wire.begin();
  Wire.setClock(100000); // 400000 https://www.arduino.cc/en/Reference/WireSetClock
 //  Wire.setClock(400000); // 400000 https://www.arduino.cc/en/Reference/WireSetClock


  //TODO: mettre une erreur si ça retourne pas la bonne valeur
  byte initDone = -1;
  // Initialization
  if (useRaw) {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(MODE_TEST));
    Wire.write(byte(0x00));
    initDone = Wire.endTransmission(I2C_ADDRESS);
    Serial.println("[Muca] Set TEST mode");
  } else {
    initDone = setRegister(0x00,MODE_NORMAL);;
    Serial.println("[Muca] Set NORMAL mode");
  }

  if (initDone == 0) {
    Serial.println("[Muca] Initialized");
    delay(100);
    isInit = true;
    delay(100);
  } else {
    Serial.println("[Muca] Error while setting up Muca. Are you sure the SDA/SCL are connected?");
  }

    // Interrupt
  pinMode(CTP_INT ,INPUT);
  #ifdef digitalPinToInterrupt
  // Serial.println("[Muca] Attachinterrupt");
   attachInterrupt(digitalPinToInterrupt(CTP_INT),interruptmuca,FALLING);
  #else
    attachInterrupt(0,touch_interrupt,FALLING);
  #endif   
}


bool Muca::updated() {
  if (!isInit) return false;
  poll();
  if (useRaw) return true;

  if (newTouch == true) {
    newTouch = false;
    return true;
  } else {
    return false;
  }
}

TouchPoint Muca::getTouch(int i) {
  return touchpoints[i];
}

bool Muca::poll() {
  if (useRaw) {
    getRawData();
  } else {
    getTouchData();
    setTouchPoints();
  }
  return true;
}

void Muca::getTouchData() {
  Wire.requestFrom(I2C_ADDRESS, TOUCH_REGISTERS);
  int register_number = 0;
  // get all register bytes when available
  while (Wire.available())
  {
    touchRegisters[register_number++] = Wire.read();
  }
}

void Muca::setTouchPoints() {
  numTouches = touchRegisters[STATUS] & 0xF;
  unsigned int registerIndex = 0;
  for (int i = 0; i < numTouches; i++) {
    // 0 1 0 1 0 0 1 1 0
    // HIGH          LOW
    // var high = b >> 4; var low = b & 0x0F;
    registerIndex = (i * 6) + 3;
    touchpoints[i].flag    = touchRegisters[registerIndex] >> 6; // 0 = down, 1 = lift up, // 2 = contact // 3 = no event
    touchpoints[i].x       = word(touchRegisters[registerIndex] & 0x0f, touchRegisters[registerIndex + 1]);
    touchpoints[i].y       = word(touchRegisters[registerIndex + 2] & 0x0f, touchRegisters[registerIndex + 3]);
    touchpoints[i].id      = touchRegisters[registerIndex + 2] >> 4;
    touchpoints[i].weight  = touchRegisters[registerIndex + 4];
    touchpoints[i].area    = touchRegisters[registerIndex + 5] >> 4;

    // Remap
    touchpoints[i].x       = map(touchpoints[i].x, 0,800, 0,width);
    touchpoints[i].y       = map(touchpoints[i].y, 0,480, 0,height);
  }
}

int Muca::getNumberOfTouches() {
  return numTouches;
}

////// RAW
//void Muca::unsureTestMode() { }



void Muca::getRawData() {

  // int startTime = millis();

  // Start scan //TODO : pas sur qu'on en a besoin
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(0x00));
  Wire.write(byte(0xc0));
  Wire.endTransmission();


  //Wait for scan complete
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  int reading = 0;
  while (1) {
    Wire.requestFrom(I2C_ADDRESS, 1);
    if (Wire.available()) {
      reading = Wire.read();
      int high_bit = (reading & (1 << 7));
      if (high_bit == 0) {
        break;
      }
    }
  }


  ////////////////////////////// Serial.print("startread:");  int tt = millis();  Serial.print(tt);
  // Read Data
  for (unsigned int rowAddr = 0; rowAddr < NUM_ROWS; rowAddr++) {

    byte result[2  * NUM_COLUMNS];

    //Start transmission
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(0x01));
    Wire.write(rowAddr);
    unsigned int st = Wire.endTransmission();
    if (st != 0) Serial.print("i2c write failed");

    delayMicroseconds(50);
    //  delayMicroseconds(50); // Wait at least 100us
    //delay(10);



    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(16)); // The address of the first column is 0x10 (16 in decimal).
    Wire.endTransmission(false);
    Wire.requestFrom(I2C_ADDRESS, 2 * NUM_COLUMNS, false); // TODO : false was added IDK why
    unsigned int g = 0;
    while (Wire.available()) {
      result[g++] = Wire.read();
    }


    //  V2  : Gain de FPS ?
    /*   Wire.beginTransmission(I2C_ADDRESS);
       for (int j = 0; j < NUM_COLUMNS; j = j + 2) {
         Wire.write(byte(16 + NUM_COLUMNS));
         Wire.requestFrom(I2C_ADDRESS, 2);
         result[j] =  Wire.read();
         result[j + 1] =  Wire.read();
       }
       Wire.endTransmission();
    */
    /*
        // V1 = ça marche !
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(byte(16)); // The address of the first column is 0x10 (16 in decimal).
        Wire.endTransmission();
        Wire.requestFrom(I2C_ADDRESS, 2 * NUM_COLUMNS, false); // TODO : falst was added IDK why
        unsigned int g = 0;
        while (Wire.available()) {
          result[g++] = Wire.read();
        }
    */



    //  V1.2 = ça marche mais meme FPS
    /*
      Wire.beginTransmission(I2C_ADDRESS);
      Wire.write(byte(16));
      Wire.endTransmission();
      Wire.requestFrom(I2C_ADDRESS, 2 * NUM_COLUMNS, false);
      unsigned int g = 0;
      for (int j = 0; j < 2 * NUM_COLUMNS; j++) {
      result[j] =  Wire.read();
      }
    */


    for (unsigned int col = 0; col < NUM_COLUMNS; col++) {
      unsigned  int output = (result[2 * col] << 8) | (result[2 * col + 1]);

#ifdef CALIBRATE
      if (calibrationSteps == CALIBRATION_MAX) {
        grid[(rowAddr * NUM_COLUMNS) +  NUM_COLUMNS - col - 1] = CALIB_THRESHOLD + output - calibrateGrid[(rowAddr * NUM_COLUMNS) +  NUM_COLUMNS - col - 1];
      } else {
        calibrateGrid[(rowAddr * NUM_COLUMNS) +  NUM_COLUMNS - col - 1] = output;
        grid[(rowAddr * NUM_COLUMNS) +  NUM_COLUMNS - col - 1] = output;
      }
#else
      grid[(rowAddr * NUM_COLUMNS) +  NUM_COLUMNS - col - 1] = output;
#endif
    }


  } // End foreachrow
  ////////////////////////////// Serial.print("end:"); Serial.println(millis() - tt);

#ifdef CALIBRATE
  if (calibrationSteps != CALIBRATION_MAX) {
    if (grid[0] < 5000) return;
    if (calibrationSteps == 0) {
      memcpy(calibrateGrid, grid, sizeof(grid));
    } else {
      for (int i = 0; i < (NUM_ROWS * NUM_COLUMNS); i++) {
        // calibrateGrid[i] = (calibrateGrid[i] & grid[i]) + ((calibrateGrid[i] ^ grid[i]) >> 1);
        calibrateGrid[i] = (calibrateGrid[i] + grid[i]) / 2;
      }
    }
    Serial.println("Calibrate");
    calibrationSteps++;
  }
#endif

}

void Muca::calibrate() {
#ifdef CALIBRATE
  calibrationSteps = 0;
#endif
}

void Muca::setGain(int gain, bool returnNormal) {
    setRegister(0x00, MODE_TEST); // ENsure test mode
    delay(100);
    setRegister(0x07, byte(gain));

    if(returnNormal) {
      setRegister(0x00, MODE_NORMAL); // ENsure test mode
      delay(100);
    }

}


//https://www.buydisplay.com/download/ic/FT5206.pdf + https://github.com/optisimon/ft5406-capacitive-touch/blob/master/CapacitanceVisualizer/FT5406.hpp
// https://github.com/hyvapetteri/touchscreen-cardiography + http://optisimon.com/raspberrypi/touch/ft5406/2016/07/13/raspberry-pi-7-inch-touchscreen-hacking/
//https://www.newhavendisplay.com/app_notes/FT5x16.pdf + https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
//https://github.com/azzazza/patch_kernel_q415/blob/master/drivers/input/touchscreen/ft5x06_ts.c
