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
#define NUM_COLUMNS       12 // Columns are doing the sensing
#define NUM_ROWS          21// Row are doing the pull ground

#define CTP_INT           2


#define byte uint8_t

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

enum MucaLine {TX, RX = NUM_ROWS};

class Muca {
  public:
    Muca();

    void init(bool interupt = true);
    void skipLine(MucaLine line, const short lineNumber[], size_t size );
    
    //CHORE
    void setGain(int val);
    void printInfo();
    void autocal();
    void printAllRegisters();
    void setNumTouchPoints();
    void setReportRate(unsigned short rate);

    // TOUCH_CONFIG
    void setConfig(byte peak, byte cal, byte thresh, byte diff);
    void setResolution(unsigned short w, unsigned short h);

    // TOUCH
    bool updated();
    int getNumberOfTouches();
    TouchPoint getTouch(int i);

    //RAW
    unsigned int grid[NUM_ROWS * NUM_COLUMNS];
    void useRawData(bool useRaw);
    void getRawData();
    unsigned int getRawData(int col, int row);

    // I2C
    byte readRegister(byte reg,short numberBytes);
    byte setRegister(byte reg, byte val);

  private:
    //CHORE
    bool poll();
    bool isInit = false;
    bool useInterrupt = true;
    bool skippedLines[NUM_ROWS + NUM_COLUMNS]; // Maximum SkippedLines

    // TOUCH
    unsigned short width = 800;
    unsigned short height = 480;
    TouchPoint touchpoints[NUM_TOUCHPOINTS];
    byte touchRegisters[TOUCH_REGISTERS];
    void getTouchData();
    void setTouchPoints();
    byte numTouches = 0;

    //RAW
    bool rawData = false;
};
