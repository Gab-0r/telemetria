#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "sd_manager.h"
#include "mpuObject.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "mpu9250.h"
#include "macros.h"
extern "C"{
    #include "nrf24_driver.h"
}

#define PIN_MISO 4
#define PIN_CS 5
#define PIN_SCK 6
#define PIN_MOSI 7

#define SPI_PORT spi0
#define READ_BIT 0x80

mpu9250 mpu(100);  //Creates an mpu object

//Interrupciones
const uint interruptPin = 2;

//Variables para la SD
FRESULT fr;
FATFS fs;
FIL fil_acel;
FIL fil_gyro;
FIL fil_magnet;
FIL fil_angles;
int ret;
char buf[100];
char filename[] = "acelData.txt";
char filename2[] = "gyroData.txt";
char filename3[] = "magnetData.txt";
char filename4[] = "anglesData.txt";
char str[100];

//Funciones
void sys_init(void);
void sys_stop();

int main() {
    int16_t magnetoVals[3];
    sys_init();//Inicialización del sistema




    sd_openfileW(fr, &fil_acel, filename);
    sd_openfileW(fr, &fil_gyro, filename2);
    sd_openfileW(fr, &fil_magnet, filename3);
    sd_openfileW(fr, &fil_angles, filename4);
   
    while(1){
        mpu.updateAngles();
        mpu.printData();     //Uses the object to print the data

        // //Escribiendo datos del acelerometro
        // sprintf(str, "%d,%d,%d\n", mpu.acceleration[0], mpu.acceleration[1], mpu.acceleration[2]);
        // sd_writefile(ret, &fil_acel, str);

        //Escribiendo datos del acelerometro
        sprintf(str, "%d,%d,%d\n", mpu.acceleration[0], mpu.acceleration[1], mpu.acceleration[2]);
        sd_writefile(ret, &fil_acel, str);

        //Escribiendo datos del giroscopio
        sprintf(str, "%d,%d,%d\n", mpu.gyro[0] - mpu.gyroCal[0], mpu.gyro[1] - mpu.gyroCal[1], mpu.gyro[2] - mpu.gyroCal[2]);
        sd_writefile(ret, &fil_gyro, str);

        //Escribiendo datos del magnetometro
        sprintf(str, "%d,%d,%d\n", mpu.magnet[0], mpu.magnet[1], mpu.magnet[2]);
        sd_writefile(ret, &fil_magnet, str);

        //Escribiendo angulos
        sprintf(str, "%d,%d\n", mpu.fullAngles[0], mpu.fullAngles[1]);
        sd_writefile(ret, &fil_angles, str);

        if(gpio_get(interruptPin) == 1){ //Haciendo polling al botón de parar de guardar los datos (PENDIENTE POR INTERRUPCION)
            sys_stop();
        }  
    }
}

void sys_init(void){
    stdio_init_all();//Inicializar I/O de C
    gpio_init(interruptPin);
    gpio_set_dir(interruptPin, GPIO_IN);

    //Delay
    sleep_ms(6000);
    //Inicializar todo el apartado de la SD
    initialize_sd();
    mount_drive(fr, &fs);
}

void sys_stop(){
    sd_closefile(fr, &fil_acel);
    sd_closefile(fr, &fil_gyro);
    sd_closefile(fr, &fil_magnet);
    sd_closefile(fr, &fil_angles);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    printf("Datos escritos");
    while (1);
}

