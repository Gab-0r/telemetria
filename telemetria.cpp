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
int ret;
char buf[100];
char filename[] = "acelData.txt";
char filename2[] = "gyroData.txt";
char str[100];

//Funciones
void sys_init(void);
void sys_stop();


// void cs_select()
// {
//     asm volatile("nop \n nop \n nop");
//     gpio_put(PIN_CS, 0); // Active low
//     asm volatile("nop \n nop \n nop");
// }

// void cs_deselect()
// {
//     asm volatile("nop \n nop \n nop");
//     gpio_put(PIN_CS, 1);
//     asm volatile("nop \n nop \n nop");
// }



// //Función para escribir registros por medio de SPI
// void write_registers(uint8_t reg, uint8_t data){
//     cs_select();
//     spi_write_blocking(SPI_PORT, &reg, 1);
//     spi_write_blocking(SPI_PORT, &data, 1);
//     cs_deselect();
// }


// void read_registers(uint8_t reg, uint8_t *buf, uint16_t len)
// {
//     // For this particular device, we send the device the register we want to read
//     // first, then subsequently read from the device. The register is auto incrementing
//     // so we don't need to keep sending the register we want, just the first.

//     reg |= READ_BIT;
//     cs_select();
//     spi_write_blocking(SPI_PORT, &reg, 1);
//     sleep_ms(10);
//     spi_read_blocking(SPI_PORT, 0, buf, len);
//     cs_deselect();
//     sleep_ms(10);
// }

// void read_magneto_registers(uint8_t reg){

//     uint8_t readVal;
//     write_registers(I2C_SLV0_ADDR, AK8963_ADDRESS|READ_FLAG); //Escribir la dirección del magnetometro

//     write_registers(I2C_SLV0_REG, reg);

//     write_registers(I2C_SLV0_CTRL, 0x86);
//     sleep_ms(2);
// }

// void mpu9250_read_raw_magneto(int16_t magnetoVals[3]) { //Used to get the raw acceleration values from the mpu
//     uint8_t buffer[6];

//     // Start reading acceleration registers from register 0x3B for 6 bytes
//     read_magneto_registers(0x03);
//     read_registers(EXT_SENS_DATA_00|READ_BIT, buffer, 3);

//     for (int i = 0; i < 3; i++) {
//         magnetoVals[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]);
//     }
// }

// void set_i2C_master(void){
//     uint8_t readVal;

//     //write_registers(, 0x20); //Activar modo I2Cmaster del I2C aux
//     read_registers(WHO_AM_I_MPU9250, &readVal, 1);
//     printf("WHO_AM_I_MPU = %d\n", readVal);


//     write_registers(USER_CTRL, 0x20); //Activar modo I2Cmaster del I2C aux
//     read_registers(USER_CTRL|READ_BIT, &readVal, 1);
//     printf("USER_CTRL = %d\n", readVal);

//     write_registers(I2C_MST_CTRL, 0x0D);//Setear reloj del I2C auxiliar
//     read_registers(I2C_MST_CTRL|READ_BIT, &readVal, 1);
//     printf("I2C_MST_CTRL = %d\n", readVal);
// }

int main() {
    int16_t magnetoVals[3];
    sys_init();//Inicialización del sistema
    //sd_openfileW(fr, &fil_acel, filename);
    //sd_openfileW(fr, &fil_gyro, filename2);

    //set_i2C_master();
   
    while(1){
        mpu.updateAngles();
        mpu.printData();
    }
    // {
    //     mpu.updateAngles();  //Uses the object to calculate the angles
    //     //mpu.printData();     //Uses the object to print the data

    //     //Escribiendo datos del acelerometro
    //     sprintf(str, "%d,%d,%d\n", mpu.acceleration[0], mpu.acceleration[1], mpu.acceleration[2]);
    //     sd_writefile(ret, &fil_acel, str);

    //     //Escribiendo datos del giroscopio
    //     sprintf(str, "%d,%d,%d\n", mpu.gyro[0] - mpu.gyroCal[0], mpu.gyro[1] - mpu.gyroCal[1], mpu.gyro[2] - mpu.gyroCal[2]);
    //     sd_writefile(ret, &fil_gyro, str);

    //     if(gpio_get(interruptPin) == 1){ //Haciendo polling al botón de parar de guardar los datos (PENDIENTE POR INTERRUPCION)
    //         sys_stop();
    //     }  
    // }

    
}

void sys_init(void){
    stdio_init_all();//Inicializar I/O de C
    gpio_init(interruptPin);
    gpio_set_dir(interruptPin, GPIO_IN);

    //Delay
    sleep_ms(6000);
    //Inicializar todo el apartado de la SD
    // initialize_sd();
    // mount_drive(fr, &fs);
    
}

void sys_stop(){
    // sd_closefile(fr, &fil_acel);
    // sd_closefile(fr, &fil_gyro);
    // gpio_put(PICO_DEFAULT_LED_PIN, 1);
    // printf("Datos escritos");
    // while (1);
}

