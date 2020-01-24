//https://www.newhavendisplay.com/appnotes/datasheets/touchpanel/FT5x16_registers.pdf
//https://www.buydisplay.com/download/ic/FT5206.pdf
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

#define CALIBRATION_MAX   3
#define CALIB_THRESHOLD   0


volatile bool newTouch = false;
void interruptmuca() {
  newTouch = true;
}

class TouchPoint {
  public: unsigned int flag;
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
    void calibrate();
    void setGain(int val);


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
    short calibrateGrid[NUM_ROWS * NUM_COLUMNS];
    int calibrationSteps = 0;
};
