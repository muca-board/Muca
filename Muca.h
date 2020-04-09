//https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
//https://www.buydisplay.com/download/ic/FT5206.pdf

// https://github.com/focaltech-systems/drivers-input-touchscreen-FTS_driver/blob/master/ft5x06.c

#include "Wire.h"

#define I2C_ADDRESS       0x38

#define MODE_NORMAL       0x00
#define MODE_TEST         0x40

// NORMAL
#define STATUS            0x02
#define TOUCH_REGISTERS   31
#define NUM_TOUCHPOINTS   4

// RAW
#define NUM_ROWS          21
#define NUM_COLUMNS       12

#define CTP_INT           2


#include "Wire.h"


class TouchPoint {
  public:
    unsigned int id;
    unsigned int flag;
    unsigned int x;
    unsigned int y;
    unsigned int weight;
    unsigned int area;
    unsigned int direction;
    unsigned int speed;
};


class Muca {
  public:
    Muca();
    void init(bool inter = true);

    bool poll();

    // CONFIG
    void setGain(int val, bool returnNormal);
    void autocal();
    void printInfo();
    void setConfig(byte peak, byte cal, byte thresh, byte diff);
    void setResolution(unsigned short w, unsigned short h);
    void printAllRegisters();
    void setNumTouchPoints();
    void setReportRate(unsigned short rate);

    // TOUCH
    bool updated();
    int getNumberOfTouches();
    TouchPoint getTouch(int i);

    //RAW
    short grid[NUM_ROWS * NUM_COLUMNS];
    void getRawData();

    // I2C
    byte readRegister(byte reg,short numberBytes);
    byte setRegister(byte reg, byte val);

  private:
    bool isInit = false;
    bool useInterrupt = true;
    unsigned short width = 800;
    unsigned short height = 480;

    // TOUCH
    TouchPoint touchpoints[NUM_TOUCHPOINTS];
    byte touchRegisters[TOUCH_REGISTERS];
    void getTouchData();
    void setTouchPoints();
    byte numTouches = 0;
};
