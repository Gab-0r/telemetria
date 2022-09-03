#include "mpuObject.h"
extern "C" {
    #include "mpu9250.h"
}

mpu9250::mpu9250(int loop) //Starts the mpu and calibrates the gyro
{
    start_spi();
    calibrate_gyro(gyroCal, loop);
    mpu9250_read_raw_accel(acceleration);
    calculate_angles_from_accel(eulerAngles, acceleration);
    timeOfLastCheck = get_absolute_time();
}

void mpu9250::updateAngles() //Calculates the angles based on the sensor readings
{   
    printf("calculando angulos...\n");
    mpu9250_read_raw_accel(acceleration);
    printf("aceleración lista...\n");
    mpu9250_read_raw_gyro(gyro);
    printf("Giroscopio listo...\n");
    mpu9250_read_raw_magneto(magnet);
    printf("magnetometro listo...\n");
    gyro[0] -= gyroCal[0];
    gyro[1] -= gyroCal[1];
    gyro[2] -= gyroCal[2];
    //calculate_angles(eulerAngles, acceleration, gyro, absolute_time_diff_us(timeOfLastCheck, get_absolute_time()));
    //timeOfLastCheck = get_absolute_time();
    printf("giroscopo listo...\n");
    convert_to_full(eulerAngles, acceleration, fullAngles);
    printf("angulos actualizados\n");
}

void mpu9250::printData() //Prints out the sensor readings and calculated values
{
    printf("imprimiendo...\n");
    //printf("%d,%d,%d\n", magnet[0], magnet[1], magnet[2]); //Acelerometro XYZ
    //printf("%d,%d,%d\n", gyro[0] - gyroCal[0], gyro[1] - gyroCal[1], gyro[2] - gyroCal[2]);//Giroscopio
    //printf("Euler. Roll = %d, Pitch = %d\n", eulerAngles[0], eulerAngles[1]);
    printf( "%d,%d\n", fullAngles[0], fullAngles[1]);
}