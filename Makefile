DBG_FLAG ?= NO_DBG

APP_ROOT_DIR = .
obj_common = ${APP_ROOT_DIR}/adxl345.o
objs = $(obj_common) ${APP_ROOT_DIR}/main.o
output = accel_msr

CC=g++
CFLAGS = -Wall -g -I$(APP_ROOT_DIR) -D$(DBG_FLAG)

all: $(objs)
	$(CC) $(objs) $(CFLAGS) -o $(output) -lwiringPi
	rm -rf *.o


clean:
	rm -rf *.o $(output) $(obj_common)
