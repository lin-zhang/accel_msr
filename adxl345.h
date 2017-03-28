#ifndef  __ADXL345
#define __ADXL345

#include <wiringPiI2C.h>
#include <math.h>
#include <stdio.h>

#define EARTH_GRAVITY_MS2    9.80665
#define SCALE_MULTIPLIER     0.004

#define DATA_FORMAT          0x31
#define BW_RATE              0x2C
#define POWER_CTL            0x2D

#define BW_RATE_1600HZ       0x0F
#define BW_RATE_800HZ        0x0E
#define BW_RATE_400HZ        0x0D
#define BW_RATE_200HZ        0x0C
#define BW_RATE_100HZ        0x0B
#define BW_RATE_50HZ         0x0A
#define BW_RATE_25HZ         0x09

#define RANGE_2G             0x00
#define RANGE_4G             0x01
#define RANGE_8G             0x02
#define RANGE_16G            0x03

#define MEASURE              0x08
#define AXES_DATA            0x32


struct acc_dat{
        double x;
        double y;
        double z;
};

class ADXL345{

public:
    int FD_ADDR;
    ADXL345(int addr);
    void enableMeasurement();
    void setBandwidthRate(int rate_flag);
    void setRange(int range_flag);
    //struct acc_dat getAxes(bool gforce = false);
    struct acc_dat getAxes(bool gforce);
};

#endif

