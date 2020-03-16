//https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
//https://www.buydisplay.com/download/ic/FT5206.pdf

// https://github.com/focaltech-systems/drivers-input-touchscreen-FTS_driver/blob/master/ft5x06.c


#include "Wire.h"

#define CTP_INT           2
#define I2C_ADDRESS       0x38


#define MODE_NORMAL       0x00
#define MODE_TEST         0x40


// NORMAL
#define TOUCH_REGISTERS   31
#define STATUS            0x02

// RAW

#define NUM_ROWS          21
#define NUM_COLUMNS       12

#define CALIBRATE         1
#define CALIBRATION_MAX   3
#define CALIB_THRESHOLD   0


#include "Wire.h"


class TouchPoint {
  public:
    unsigned int flag;
    unsigned int x;
    unsigned int y;
    unsigned int weight;
    unsigned int area;
    unsigned int id;
};


class Muca {
  public:
    Muca();
    void init(bool raw = false);

    bool poll();

    // TOUCH
    bool updated();
    int getNumberOfTouches();
    TouchPoint getTouch(int i);

    //RAW
    void pollRaw();
    bool useRaw = false;
    short grid[NUM_ROWS * NUM_COLUMNS];
    void setGain(int val);
    void autocal();

    void setupTrucs();
    void printInfo();
    void testconfig();
    void setConfig(byte peak, byte cal, byte thresh, byte diff );


    void calibrate();

  private:
    bool isInit = false;

    // TOUCH
    TouchPoint touchpoints[5];
    byte touchRegisters[TOUCH_REGISTERS];
    void getTouchData();
    void setTouchPoints();
    byte numTouches = 0;

    //RAW
    void getRawData();

    #ifdef CALIBRATE
    short calibrateGrid[NUM_ROWS * NUM_COLUMNS];
    int calibrationSteps = 0;
    #endif
};
