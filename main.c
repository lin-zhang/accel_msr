#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include "adxl345.h"

int fd_adxl345;
struct acc_dat accel_data;

struct timeval tv, t_start;

char file_name[64];

bool stop_flag = false;

void time_delay(uint32_t sec, uint32_t msec){
    struct timeval t;
    t.tv_sec = sec;
    t.tv_usec = msec*1000;
    select(0, NULL, NULL, NULL, &t);
}

void handleSigInt(int param) {
        printf("[CTRL-C] Data acquisition terminated, logging data to file ...\n");
        stop_flag = true;
}

int main(int argc, char* argv[]){

    fd_adxl345 = wiringPiI2CSetup(0x53);
    ADXL345 adxl345(fd_adxl345);
    adxl345.setRange(RANGE_2G);
    adxl345.setBandwidthRate(BW_RATE_400HZ);

/* Register interrupt signal to terminate the aquisition, 
   so that the while loop below can be stopped normally  */
    signal(SIGINT, handleSigInt);

    char s_date_time[64]; 
    char std_date_time[64]; 
    struct tm *sTm;
    time_t now = time (0);
    sTm = gmtime (&now);

    int delay=atoi(argv[2]);

    strftime (s_date_time, sizeof(s_date_time), "_%Y%m%d-%H%M%S", sTm);
    strftime (std_date_time, sizeof(std_date_time), " %Y-%m-%d %H:%M:%S", sTm);
    printf ("[INFO] Program started at %s \n", std_date_time);
    
    gettimeofday(&tv,NULL);
    t_start.tv_sec = tv.tv_sec;
    t_start.tv_usec = tv.tv_usec;
    unsigned long start_time_us = 1000000 * tv.tv_sec + tv.tv_usec;
    unsigned long elapsed_time_us = 0;

    sprintf(file_name ,"%s", argv[1]); 

    strcat(file_name, s_date_time);
    strcat(file_name, ".txt");

    printf("[INFO] Data file saved as: %s\n", file_name);
    

    FILE *fd = fopen(file_name, "w");
    if (fd == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    while(!stop_flag){
        //get measurement
        accel_data = adxl345.getAxes(true);
        gettimeofday(&tv, NULL);

        fprintf(fd, "%10.6f %8.3f %8.3f %8.3f\n", 
                        (double)elapsed_time_us/1000000.0, (double)accel_data.x, (double)accel_data.y, (double)accel_data.z);
        time_delay(0,delay);
        elapsed_time_us = 1000000 * tv.tv_sec + tv.tv_usec - start_time_us;
#ifdef DBG
        printf("%10.6f %8.3f %8.3f %8.3f\n", 
                        (double)elapsed_time_us/1000000.0, (double)accel_data.x, (double)accel_data.y, (double)accel_data.z);
#endif
    }

    fclose(fd);

    return 0;
}

///* print some text */
//const char *text = "Write this to the file";
//fprintf(f, "Some text: %s\n", text);
//
///* print integers and floats */
//int i = 1;
//float py = 3.1415927;
//fprintf(f, "Integer: %d, float: %f\n", i, py);
//
///* printing single chatacters */
//char c = 'A';
//
//fprintf(f, "A character: %c\n", c);
