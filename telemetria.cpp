#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "sd_manager.h"
#include "mpuObject.h"

mpu9250 mpu(100);  //Creates an mpu object

int main() {
    stdio_init_all();

    sleep_ms(2000);

    printf("Hello, MPU9250! Reading raw data from registers via SPI...\n");

    while (1)
    {
        mpu.updateAngles();  //Uses the object to calculate the angles
        mpu.printData();     //Uses the object to print the data
    }
}