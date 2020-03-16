#define FT5206_ID_G_THGROUP                 0x80 // Valid touching detect threshold
#define FT5206_ID_G_THPEAK                  0x81 // Valid touching peak detect threshold
#define FT5206_ID_G_THCAL                   0x82 // The threshold when calculating the focus of touching
#define FT5206_ID_G_THWATER                 0x83 // The threshold when there is surface water
#define FT5206_ID_G_THTEMP                  0x84 // The threshold of temperature compensation
#define FT5206_ID_G_CTRL                    0x86 // Power control mode
#define FT5206_ID_G_TIME_ENTER_MONITOR      0x87 // The timer of entering monitor status
#define FT5206_ID_G_PERIODACTIVE            0x88 // Period Active
#define FT5206_ID_G_PERIODMONITOR           0x89 // The timer of entering idle while in monitor status
#define FT5206_ID_G_AUTO_CLB_MODE           0xA0 // Auto calibration mode
 
#define FT5206_TOUCH_LIB_VERSION_H          0xA1 // Firmware Library Version H byte
#define FT5206_TOUCH_LIB_VERSION_L          0xA2 // Firmware Library Version L byte
#define FT5206_ID_G_CIPHER                  0xA3 // Chip vendor ID
#define FT5206_G_MODE                       0xA4 // The interrupt status to host
#define FT5206_ID_G_PMODE                   0xA5 // Power Consume Mode
#define FT5206_FIRMID                       0xA6 // Firmware ID
#define FT5206_ID_G_STATE                   0xA7 // Running State
#define FT5206_ID_G_FT5201ID                0xA8 // CTPM Vendor ID
#define FT5206_ID_G_ERR                     0xA9 // Error Code
#define FT5206_ID_G_CLB                     0xAA // Configure TP module during calibration in Test Mode
#define FT5206_ID_G_B_AREA_TH               0xAE // The threshold of big area
#define FT5206_LOG_MSG_CNT                  0xFE // The log MSG count
#define FT5206_LOG_CUR_CHA                  0xFF // Current character of log message, will point to the next
                                               // character when one character is read.
 
#define FT5206_GEST_ID_MOVE_UP              0x10
#define FT5206_GEST_ID_MOVE_LEFT            0x14
#define FT5206_GEST_ID_MOVE_DOWN            0x18
#define FT5206_GEST_ID_MOVE_RIGHT           0x1c
#define FT5206_GEST_ID_ZOOM_IN              0x48
#define FT5206_GEST_ID_ZOOM_OUT             0x49
#define FT5206_GEST_ID_NO_GESTURE           0x00
 
#define FT5206_EVENT_FLAG_PUT_DOWN          0x00
#define FT5206_EVENT_FLAG_PUT_UP            0x01
#define FT5206_EVENT_FLAG_CONTACT           0x02
#define FT5206_EVENT_FLAG_RESERVED          0x03
 
#define FT5206_ID_G_POLLING_MODE            0x00
#define FT5206_ID_G_TRIGGER_MODE            0x01
 
#define FT5206_ID_G_PMODE_ACTIVE            0x00
#define FT5206_ID_G_PMODE_MONITOR           0x01
#define FT5206_ID_G_PMODE_HIBERNATE         0x03
 
#define FT5206_ID_G_STATE_CONFIGURE         0x00
#define FT5206_ID_G_STATE_WORK              0x01
#define FT5206_ID_G_STATE_CALIBRATION       0x02
#define FT5206_ID_G_STATE_FACTORY           0x03
#define FT5206_ID_G_STATE_AUTO_CALIBRATION  0x04