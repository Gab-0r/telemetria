#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "sd_manager.h"
#include "mpuObject.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

mpu9250 mpu(100);  //Creates an mpu object

//Interrupciones
const uint interruptPin = 2;

//Variables para la SD
FRESULT fr;
FATFS fs;
FIL fil_acel;
FIL fil_gyro;
int ret;
char buf[100];
char filename[] = "acelData.txt";
char filename2[] = "gyroData.txt";
char str[100];

//Funciones
void sys_init(void);
void sys_stop();

int main() {
    sys_init();//Inicialización del sistema
    sd_openfileW(fr, &fil_acel, filename);
    sd_openfileW(fr, &fil_gyro, filename2);
    while(1)
    {
        mpu.updateAngles();  //Uses the object to calculate the angles
        mpu.printData();     //Uses the object to print the data

        // //Escribiendo datos del acelerometro
        // sprintf(str, "%d,%d,%d\n", mpu.acceleration[0], mpu.acceleration[1], mpu.acceleration[2]);
        // sd_writefile(ret, &fil_acel, str);

        // //Escribiendo datos del giroscopio
        // sprintf(str, "%d,%d,%d\n", mpu.gyro[0] - mpu.gyroCal[0], mpu.gyro[1] - mpu.gyroCal[1], mpu.gyro[2] - mpu.gyroCal[2]);
        // sd_writefile(ret, &fil_gyro, str);

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
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    printf("Datos escritos");
    while (1);
}

