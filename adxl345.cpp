#include "./adxl345.h"

ADXL345::ADXL345(int addr)
{
    FD_ADDR = addr;
    setBandwidthRate(BW_RATE_400HZ);
    setRange(RANGE_2G);
    enableMeasurement();

}
void ADXL345::enableMeasurement()
{
     wiringPiI2CWriteReg8(FD_ADDR,POWER_CTL,MEASURE);
}

void ADXL345::setBandwidthRate(int rate_flag)
{
     //printf("%d",rate_flag);
     wiringPiI2CWriteReg8(FD_ADDR,BW_RATE,rate_flag);
}

void ADXL345::setRange(int range_flag)
{
        int value;
        value = wiringPiI2CReadReg8(FD_ADDR, DATA_FORMAT);
        value &= ~0x0F;
        value |= range_flag;
        value |= 0x08;

//        printf("set range value : %d",value);
        wiringPiI2CWriteReg8(FD_ADDR, DATA_FORMAT, value);
}

struct acc_dat ADXL345::getAxes(bool gforce = false)
{
        double dx,dy,dz;
        struct acc_dat dat;
        int x,y,z;
        char bytes[6];
        bytes[0] = wiringPiI2CReadReg8(FD_ADDR, AXES_DATA);
        bytes[1] = wiringPiI2CReadReg8(FD_ADDR, AXES_DATA+1);
        bytes[2] = wiringPiI2CReadReg8(FD_ADDR, AXES_DATA+2);
        bytes[3] = wiringPiI2CReadReg8(FD_ADDR, AXES_DATA+3);
        bytes[4] = wiringPiI2CReadReg8(FD_ADDR, AXES_DATA+4);
        bytes[5] = wiringPiI2CReadReg8(FD_ADDR, AXES_DATA+5);

        //printf("%d\n",bytes[0]);
        //printf("%d\n",bytes[1]);
        x = bytes[0] | (bytes[1] << 8);
        if(x & (1<<15))
            x = x - (1<<16);
        //printf("%d\n",x);

        y = bytes[2] | (bytes[3] << 8);
        if(y & (1<<15))
            y = y - (1<<16);

        z = bytes[4] | (bytes[5] << 8);
        if(z & (1<<15))
            z = z - (1<<16);

        dx = (double)x * SCALE_MULTIPLIER;
        dy = (double)y * SCALE_MULTIPLIER;
        dz = (double)z * SCALE_MULTIPLIER;

        if (gforce == false)
        {
            dx = dx * EARTH_GRAVITY_MS2;
            dy = dy * EARTH_GRAVITY_MS2;
            dz = dz * EARTH_GRAVITY_MS2;
        }
        dat.x = (dx);
        dat.y = (dy);
        dat.z = (dz);

        return dat;
}
