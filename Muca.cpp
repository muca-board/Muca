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


void Muca::setConfig(byte peak, byte cal, byte thresh, byte diff ) {

  // range 0 to 80
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x80);  
    Wire.write(peak);  // 0x46 = 70 // 0 to 80.
    Wire.endTransmission(I2C_ADDRESS);


  //Valid touching detect threshold   
    //4? ID_G_THPEAK default 60
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x81);  
    Wire.write(cal);  // RECOMMANDED 3C
    Wire.endTransmission(I2C_ADDRESS);


  // Touch focus threshold               ID_G_THCAL 
  // defaut 16 sensitivity in the range from 0 to 31. Note that lower values indicate higher sensitivity
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x82);  
    Wire.write(thresh);  // 0x1D = 29
    Wire.endTransmission(I2C_ADDRESS);


  // Touch difference threshold      
  //*16  The actual value must be 16 times of the register’s value.   ID_G_THDIFF 
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x85);  
    Wire.write(diff); // 0xA0 = 160 // deut 20
    Wire.endTransmission(I2C_ADDRESS);



Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0xA7);  
    Wire.write(byte(1));   // 00 true // ff false
    Wire.endTransmission(I2C_ADDRESS);

}


void Muca::testconfig() {

Serial.println("---");
Serial.println("CONF");
  //Valid touching detect threshold  /4 default 120/4     ID_G_THGROUP
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x80);  
    Wire.write(0x46);  // 0x46 = 70 // 0 to 80.
    Wire.endTransmission(I2C_ADDRESS);


  //Valid touching detect threshold   /4? ID_G_THPEAK default 60
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x81);  
    Wire.write(0x3C);  // RECOMMANDED 3C
    Wire.endTransmission(I2C_ADDRESS);


  // Touch focus threshold               ID_G_THCAL // defaut 16 sensitivity in the range from 0 to 31. Note that lower values indicate higher sensitivity
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x82);  
    Wire.write(0x10);  // 0x1D = 29
    Wire.endTransmission(I2C_ADDRESS);


  // Touch difference threshold      /16  The actual value must be 16 times of the register’s value.   ID_G_THDIFF 
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x85);  
    Wire.write(0xA0); // 0xA0 = 160 // deut 20
    Wire.endTransmission(I2C_ADDRESS);






// SETUP AUTO CALIBRATION
  Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0xA0);  
    Wire.write(0x00);   // 00 true // ff false
    Wire.endTransmission(I2C_ADDRESS);


/*
    // auto calibration state
      Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0xA7);  
    Wire.write(byte(2));   // 00 true // ff false
    Wire.endTransmission(I2C_ADDRESS);


    delay(1000);

    // working mode
      Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0xA7);  
    Wire.write(byte(1));   // 00 true // ff false
    Wire.endTransmission(I2C_ADDRESS);

*/

}

void Muca::printInfo() {

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0xA7);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS,1);
  byte state = Wire.read();
  Serial.print("state:");
  Serial.println(state);


Serial.println("---");
byte registers[10];
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x80);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS,1);
  registers[0] = Wire.read();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x81);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS,1);
  registers[1] = Wire.read();



  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x82);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS,1);
  registers[2] = Wire.read();

  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0x85); // default a0 = 160
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS,1);
  registers[3] = Wire.read();


  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(0xA0);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDRESS,1);
  registers[4] = Wire.read();



 Serial.print("ID_G_THGROUP\t");
    Serial.println(registers[0]);
    Serial.print("ID_G_THPEAK\t");
    Serial.println(registers[1]);
    Serial.print("ID_G_THCAL\t");
    Serial.println(registers[2]);
    Serial.print("ID_G_THDIFF\t");
    Serial.println(registers[3]);
    Serial.print("AUTO_CLB_MODE\t");
    Serial.println(registers[4]);

/*
    byte registers[10];
    Wire.requestFrom(0x38, 0x80, true); 
    int register_number = 0;
    // get all register bytes when available
    while(Wire.available())
    {
      registers[register_number++] = Wire.read();
      delay(5);
    }
    delay(10);
    // Might be that the interpretation of high/low bit is not same as major/minor version...
    Serial.print("ID_G_THGROUP\t");
    Serial.println(registers[0]);
    Serial.print("ID_G_THPEAK\t");
    Serial.println(registers[1]);
    Serial.print("ID_G_THCAL\t");
    Serial.println(registers[2]);
    Serial.print("ID_G_THWATER\t");
    Serial.println(registers[3]);
    Serial.print("ID_G_THWATER\t");
    Serial.println(registers[4]);
    Serial.print("ID_G_TEMP\t");
    Serial.println(registers[5]);
    Serial.print("ID_G_THDIFF\t");
    Serial.println(registers[6]);
*/



/*
    byte registers[0xFE];
    Wire.requestFrom(0x38, 0xFE); 
    int register_number = 0;
    // get all register bytes when available
    while(Wire.available())
    {
      registers[register_number++] = Wire.read();
      delay(5);
    }
    delay(10);
    // Might be that the interpretation of high/low bit is not same as major/minor version...
    Serial.print("Library version: ");
    Serial.print(registers[0xa1]);
    Serial.print(".");
    Serial.print(registers[0xa2]);
    Serial.println(".");

*/
}


void Muca::setupTrucs() {

}


void Muca::autocal() {

    int error = 0;
    unsigned char uc_temp;
    unsigned char i ;
 
    Serial.println("[FTS] start auto CLB.");
    delay(200);
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(MODE_TEST);
    Wire.endTransmission(I2C_ADDRESS);
    delay(100);                       //make sure already enter factory mode

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x02);
    Wire.write(0x04);
    Wire.endTransmission(I2C_ADDRESS);


    delay(300);


    bool done = false;
    for(i=0;i<100;i++)
    {
      if(done) break;
        Wire.beginTransmission(I2C_ADDRESS);
        Wire.write(0x00);
        Wire.endTransmission();
       byte reading = Wire.read();
       Serial.println(reading);

        if ( ((reading & 0x70)>>4) == 0)    //return to normal mode, calibration finish
        {
           done = true;
            break;
        }

        delay(200);
        Serial.print("[FTS] waiting calibration ");
        Serial.println(i);

   }
 
    Serial.println("[FTS] calibration OK.");
 
    delay(300);

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(0x40);
    error = Wire.endTransmission(I2C_ADDRESS);

    if(error) Serial.print("error"); Serial.println(error);

    delay(100);                       //make sure already enter factory mode
    
    Wire.beginTransmission(I2C_ADDRESS); // save
    Wire.write(0x02);
    Wire.write(0x05);
    error = Wire.endTransmission(I2C_ADDRESS);

    if(error) Serial.print("error"); Serial.println(error);

    delay(300);

    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission(I2C_ADDRESS);


   delay(300);
    Serial.println("[FTS] store CLB result OK.");
}

void Muca::init(bool raw = false) {
  useRaw = raw;
  digitalWrite(SDA, LOW);
  digitalWrite(SCL, LOW);

  // Interrupt
  /*
    pinMode(2 , INPUT);
    attachInterrupt(0, interruptmuca, FALLING);
  */

  Wire.begin();
  //Wire.setClock(100000); // 400000 https://www.arduino.cc/en/Reference/WireSetClock
  Wire.setClock(400000); // 400000 https://www.arduino.cc/en/Reference/WireSetClock


 //TODO: mettre une erreur si ça retourne pas la bonne valeur
  byte initDone = -1;
  // Initialization
  if (useRaw) {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(byte(MODE_TEST));
       Wire.write(byte(0x00));
 initDone =Wire.endTransmission(I2C_ADDRESS);
  } else {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(0);
    Wire.write(MODE_NORMAL);
    initDone = Wire.endTransmission(I2C_ADDRESS);
  }

  if(initDone == 0) {
    Serial.println("Muca initialized");
    delay(100);
    isInit = true;
     delay(100);
  } else {
    Serial.println("Error while setting up Muca. Are you sure the SDA/SCL are connected?");
  }
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
    // touchpoints[i].flag = touchRegisters[registerIndex] les deux premiers bits
    touchpoints[i].x       = word(touchRegisters[registerIndex] & 0x0f, touchRegisters[registerIndex + 1]);
    touchpoints[i].y       = word(touchRegisters[registerIndex + 2] & 0x0f, touchRegisters[registerIndex + 3]);
    touchpoints[i].id      = touchRegisters[registerIndex + 2] >> 4;
    touchpoints[i].weight  = touchRegisters[registerIndex + 4];
    touchpoints[i].area    = touchRegisters[registerIndex + 5] >> 4;
  }
}

int Muca::getNumberOfTouches() {
  return numTouches;
}

////// RAW
//void Muca::unsureTestMode() { }



void Muca::getRawData() {

  int startTime = millis();

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
    if (st < 0) Serial.print("i2c write failed");

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

void Muca::setGain(int gain) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(byte(0x07));
  Wire.write(byte(gain));
  Wire.endTransmission();
}


//https://www.buydisplay.com/download/ic/FT5206.pdf + https://github.com/optisimon/ft5406-capacitive-touch/blob/master/CapacitanceVisualizer/FT5406.hpp
// https://github.com/hyvapetteri/touchscreen-cardiography + http://optisimon.com/raspberrypi/touch/ft5406/2016/07/13/raspberry-pi-7-inch-touchscreen-hacking/
//https://www.newhavendisplay.com/app_notes/FT5x16.pdf + https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
//https://github.com/azzazza/patch_kernel_q415/blob/master/drivers/input/touchscreen/ft5x06_ts.c
