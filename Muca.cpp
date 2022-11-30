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

int Muca::getRegister(byte reg) {
  // Read a register
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(reg);
  unsigned int st = Wire.endTransmission(false);
  if (st != 0)  {
    Serial.println("[Muca] i2c write failed");
  }
  Wire.requestFrom(I2C_ADDRESS, 1);
  byte result;
  if (Wire.available()) {
    result = Wire.read();
  }
  return result;
}

void Muca::getRegisters(byte reg, byte size, byte * buffer) {
  // Read n registers
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(reg);
  unsigned int st = Wire.endTransmission(false);
  if (st != 0)  {
    Serial.println("[Muca] i2c write failed");
  }
  Wire.requestFrom(I2C_ADDRESS, size);
  byte i = 0;
  while (Wire.available()) {
    buffer[i++] = Wire.read();
  }
}

void Muca::setConfig(byte touchdetectthresh, byte touchpeak, byte threshfocus, byte threashdiff ) {
  setRegister(0x80, touchdetectthresh);
  setRegister(0x81, touchpeak);
  setRegister(0x82, threshfocus);
  setRegister(0x85, threashdiff);

  setRegister(0xA0, 0x00); 		// enable auto calib

  setRegister(0x00,MODE_NORMAL); 	// DEVICE_MODE : NORMAL
}

// The gain value can be changed from 1 to 31
// Return to normal mode in RawData is not activated           
void Muca::setGain(int gain) {
    setRegister(0x00, MODE_TEST); 	// Ensure test mode
    delay(100);
    setRegister(0x07, byte(gain));
#ifdef DEBUG
    Serial.print("[Muca] Gain set to ");
    Serial.println((gain));
#endif

    if(!rawData) {
      setRegister(0x00, MODE_NORMAL);
      delay(100);
    }
}


void Muca::printAllRegisters() {
  setRegister(0x00, MODE_NORMAL);
  // setRegister(0xA7, 0x03); 		// ID_G_ STATE   FACTORY

  byte prev = 0;
  Serial.print("[Muca] ");
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

void Muca::printInfo() {

  Serial.print("[Muca] MODE\t");
  Serial.print(readRegister(0xA7, 1));
  Serial.print("\t");

  Serial.print("[Muca] ID_G_THGROUP\t");
  Serial.print(readRegister(0x80, 1));
  Serial.print("\t");

  Serial.print("[Muca] ID_G_THPEAK\t");
  Serial.print(readRegister(0x81, 1));
  Serial.print("\t");

  Serial.print("[Muca] ID_G_THCAL\t");
  Serial.print(readRegister(0x82, 1));
  Serial.print("\t");

  Serial.print("[Muca] ID_G_THDIFF\t");
  Serial.print(readRegister(0x85, 1));
  Serial.print("\t");

  Serial.print("[Muca] AUTO_CLB_MODE\t");
  Serial.print(readRegister(0xA0, 1));
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

    if ( ((reading & 0x70) >> 4) == 0x0)	// return to normal mode, calibration finish
    {
      done = true;
#ifdef DEBUG
      Serial.println("[Muca] Calibration done!");
#endif
      break;
    }

    delay(200);
#ifdef DEBUG
    Serial.println("[Muca] Waiting calibration...");
#endif
  }

  //Serial.println("[Muca] Calibration OK.");

  delay(300);

  error = setRegister(0x00,MODE_TEST);

#ifdef DEBUG
  if (error != 0) { Serial.print("[Muca] Calibration Error"); Serial.println(error);}
#endif

  delay(100);                       		// make sure already enter factory mode

  error = setRegister(0x02,0x5); 		// Save calibration result

#ifdef DEBUG
  if (error != 0) {Serial.print("[Muca] Calibration Error"); Serial.println(error);}
#endif
  delay(300);

  setRegister(0x00,MODE_NORMAL); 


  delay(300);
#ifdef DEBUG
  Serial.println("[Muca] Store CLB result OK.");
#endif
}

void Muca::selectLines(bool RX[NUM_RX], bool TX[NUM_TX]) {
  // Reduce the number of lines scanned in raw mode if not used
  uint8_t count = 0;
  for(uint8_t i=0; i<NUM_RX; i++) { 
	  RX_lines[i] = RX[i];
    if (RX[i] == true) count++;
  }
  num_RX = count;
  count=0;
  for(uint8_t i=0; i<NUM_TX; i++) { 
	  TX_lines[i] = TX[i];
    if (TX[i] == true) count++;
  }
  num_TX = count;
#ifdef DEBUG
  Serial.print("Num_TX : ");
  Serial.println(num_TX);
  Serial.print("Num_RX : ");
  Serial.println(num_RX);
#endif
}


Muca::Muca() {}

bool Muca::init(bool interrupt) {

  useInterrupt = interrupt;
 
  //Setup I2C
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);

  Wire.begin();
  Wire.setClock(400000); 			// https://www.arduino.cc/en/Reference/WireSetClock

  Wire.setTimeout(200);

  byte initDone = -1;
  initDone = setRegister(0x00,MODE_NORMAL);
  //Serial.println("[Muca] Set NORMAL mode");

  if (initDone == 0) {
    //Serial.println("[Muca] Initialized");
    delay(100);
    isInit = true;
    delay(100);
  } else {
    Serial.println("[Muca] Error while setting up Muca. Are you sure the SDA/SCL are connected?");
    return 1;
  }

    // Interrupt
  if(useInterrupt) {
    pinMode(CTP_INT ,INPUT);
    #ifdef digitalPinToInterrupt
    // Serial.println("[Muca] Attachinterrupt");
     attachInterrupt(digitalPinToInterrupt(CTP_INT),interruptmuca,FALLING);
    #else
      attachInterrupt(0,touch_interrupt,FALLING);
    #endif   
  }
  setRegister(0xA7,0x04); // Set autocalibration
}


bool Muca::update() {
  if (!isInit)
    return false;

  if(!useInterrupt) {
    if(rawData) {
      getRawData();
    } else {
      getTouchData();
      setTouchPoints();
    }
    return true;
  }
  else {
      if (newTouch == true) {
        getTouchData();
        setTouchPoints();
        newTouch = false;
        return true;
      } else {
        return false;
      }
  }
}

//////////////////////////////
//    TOUCH POINT DATA
//////////////////////////////


TouchPoint Muca::getTouch(int i) {
  return touchpoints[i];
}


void Muca::getTouchData() {
  Wire.requestFrom(I2C_ADDRESS, TOUCH_REGISTERS);

  int register_number = 0;
  // get all register bytes when available
  while (Wire.available() >0)
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
    registerIndex            = (i * 6) + 3;
    touchpoints[i].flag      = touchRegisters[registerIndex] >> 6; // 0 = down, 1 = lift up, // 2 = contact // 3 = no event
    touchpoints[i].x         = word(touchRegisters[registerIndex] & 0x0f, touchRegisters[registerIndex + 1]);
    touchpoints[i].y         = word(touchRegisters[registerIndex + 2] & 0x0f, touchRegisters[registerIndex + 3]);
    touchpoints[i].id        = touchRegisters[registerIndex + 2] >> 4;
    touchpoints[i].weight    = touchRegisters[registerIndex + 4];
    touchpoints[i].area      = touchRegisters[registerIndex + 5] >> 4;
    touchpoints[i].direction = touchRegisters[registerIndex + 5] >> 4;
    touchpoints[i].speed     = touchRegisters[registerIndex + 5] >> 4;

    // Remap
    touchpoints[i].x         = map(touchpoints[i].x, 0,800, 0,width);
    touchpoints[i].y         = map(touchpoints[i].y, 0,480, 0,height);
  }
}

int Muca::getNumberOfTouches() {
  return numTouches;
}


void Muca::setReportRate(unsigned short rate) {
  if(rate > 14) rate = 14;
  else if(rate < 3) rate = 3;

  setRegister(0x88, rate);
  setRegister(0x00,MODE_NORMAL);
}

//////////////////////////////
//        RAW DATA
//////////////////////////////

void Muca::useRawData(bool useRaw) {
  rawData = useRaw;
  useInterrupt = false;
  grid = new unsigned int[num_TX*num_RX];   // Allocate only if raw mode is used, and after selectLines() was called
  if(isInit && useRaw) {
    //setRegister(0x00, MODE_TEST);
    setRegister(byte(0x00),byte(0xC0)); 	    // Set Test/Read raw mode and Data Read Toggle mode
#ifdef DEBUG
    Serial.println("[Muca] Set TEST mode");
#endif
  }
}

void Muca::getRawData() {
  setRegister(byte(0x00),byte(0xC0)); 	    // Set Test/Read raw mode and Data Read Toggle mode
  byte buffer[2  * NUM_RX];
  byte gridTxAddr = 0, gridRxAddr = 0;      // index to write selected lines only in grid[]
  // Read each activated line
  for (unsigned int txAddr = 0; txAddr < NUM_TX; txAddr++) {
    if(TX_lines[txAddr] == true) {
      setRegister(0x01, NUM_TX - 1 - txAddr); // TX lines seem to be inverted
      delayMicroseconds(50);
      getRegisters(0x10, 2*NUM_RX, buffer);

      gridRxAddr = 0;
      for (unsigned int rxAddr = 0; rxAddr < NUM_RX; rxAddr++) {
        if(RX_lines[rxAddr] == true) {      // Ignore deactivated column (rx)
          grid[(gridTxAddr * num_RX) + gridRxAddr ] = (buffer[2 * rxAddr] << 8) | (buffer[2 * rxAddr + 1]);
          gridRxAddr++;
        }
      }
      gridTxAddr++;
    }
  }
}

unsigned int Muca::getRawData(int rx, int tx) {

  newTouch = true;
  rawData = true;

  unsigned int data = 0;

  if(rx == 0 || tx == 0) {
    Serial.println(F("[Muca] The rx number or raw number must be higher than 0"));
    return 0;
  }

  setRegister(byte(0x00),byte(0xc0));


// Read Data
  int rxAddr =   NUM_RX- (rx-1) -1; // We invert because the pinout is inverted
  int txAddr =   (tx-1);

  byte result[2];
  //Start transmission
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(0x01));
  Wire.write(txAddr);
  unsigned int st = Wire.endTransmission();
  if (st != 0) Serial.println("[Muca] i2c write failed");

  delayMicroseconds(50);

  Wire.beginTransmission(I2C_ADDRESS);
 // Wire.write(0x10); // The address of the first rx is 0x10 (16 in decimal).
  Wire.write(byte(0x10) + rxAddr*2); 
  Wire.endTransmission(false);
  Wire.requestFrom(I2C_ADDRESS, 2);
  unsigned int g = 0;
  while (Wire.available()) {
    result[g++] = Wire.read();
  }

  data = (result[0] << 8) | (result[1]);

  return data;
}


//https://www.buydisplay.com/download/ic/FT5206.pdf + https://github.com/optisimon/ft5406-capacitive-touch/blob/master/CapacitanceVisualizer/FT5406.hpp
// https://github.com/hyvapetteri/touchscreen-cardiography + http://optisimon.com/raspberrypi/touch/ft5406/2016/07/13/raspberry-pi-7-inch-touchscreen-hacking/
//https://www.newhavendisplay.com/app_notes/FT5x16.pdf + https://support.newhavendisplay.com/hc/en-us/article_attachments/4414413014551/FT5x16_registers.pdf
//https://github.com/azzazza/patch_kernel_q415/blob/master/drivers/input/touchscreen/ft5x06_ts.c
