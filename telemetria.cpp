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
const uint interruptPin = 22;

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
    printf("termina inicialización del sistema\n");
    //Inicialización del enlace RF
    // Pins del modulo  RF
    pin_manager_t pins_rf = { 
        .copi = 3, 
        .cipo = 4, 
        .sck = 2,
        .csn = 5, 
        .ce = 6 
    };

    //Estructura para la configuración del módulo RF
    nrf_manager_t my_config = {
    // AW_3_BYTES, AW_4_BYTES, AW_5_BYTES
    .address_width = AW_5_BYTES,
    // dynamic payloads: DYNPD_ENABLE, DYNPD_DISABLE
    .dyn_payloads = DYNPD_ENABLE,
    // retransmission delay: ARD_250US, ARD_500US, ARD_750US, ARD_1000US
    .retr_delay = ARD_500US, 
    // retransmission count: ARC_NONE...ARC_15RT
    .retr_count = ARC_10RT,
    // data rate: RF_DR_250KBPS, RF_DR_1MBPS, RF_DR_2MBPS
    .data_rate = RF_DR_1MBPS,
    // RF_PWR_NEG_18DBM, RF_PWR_NEG_12DBM, RF_PWR_NEG_6DBM, RF_PWR_0DBM
    .power = RF_PWR_NEG_12DBM,
    // RF Channel 
    .channel = 120,
  };

    //Baudrate para el módulo RF
    uint32_t my_baudrate = 5000000;

    //Objeto RF
    static nrf_client_t my_nrf;

    //Inicialización de RF
    nrf_driver_create_client(&my_nrf);

    // configure GPIO pins and SPI
    my_nrf.configure(&pins_rf, my_baudrate);

    // not using default configuration (my_nrf.initialise(NULL)) 
    my_nrf.initialise(&my_config);

    // set to Standby-I Mode
    my_nrf.standby_mode();

    //Estructura de datos para enviar los datos de la IMU por el pipe0
    typedef struct payload_zero_s {
        uint8_t tagAcel;
        int16_t acelX;
        int16_t acelY;
        int16_t acelZ;
        uint8_t tagGyro;
        int16_t gyroX;
        int16_t gyroY;
        int16_t gyroZ;
        uint8_t tagMag;
        int16_t magX;
        int16_t magY;
        int16_t magZ;
    } payload_zero_t;

    //Estructura de datos para enviar los datos del sensor de viento por el pipe1
    typedef struct payload_one_s {
        uint8_t tagSpeed;
        int16_t windSpeed;
        uint8_t tagDir;
        int16_t windDir; 
    } payload_one_t;

    //Estructura de datos para enviar datos por el pipe2
    typedef struct payload_two_s { uint8_t one; uint8_t two; } payload_two_t;

    //Archivos donde se guardaran los datos
    //sd_openfileW(fr, &fil_acel, filename);
    //sd_openfileW(fr, &fil_gyro, filename2);
    //sd_openfileW(fr, &fil_magnet, filename3);
    //sd_openfileW(fr, &fil_angles, filename4);

    // result of packet transmission
    fn_status_t success;

    uint64_t time_sent = 0; // time packet was sent
    uint64_t time_reply = 0; // response time after packet sent

    const uint8_t pipezero_addr[5] = {0x37,0x37,0x37,0x37,0x37};
    printf("Configuración completada\n");
    while(1){
        mpu.updateAngles();
        //mpu.printData();     //Uses the object to print the data
        printf("preparando payload cero\n");
        //Escribiendo/Enviando datos de la IMU
        payload_zero_t payload_zero ={
            .tagAcel = 0xFF,
            .acelX = mpu.acceleration[0],
            .acelY = mpu.acceleration[1],
            .acelZ = mpu.acceleration[2],
            .tagGyro = 0xFE,
            .gyroX = (mpu.gyro[0] - mpu.gyroCal[0]),
            .gyroY = (mpu.gyro[1] - mpu.gyroCal[1]),
            .gyroZ = (mpu.gyro[2] - mpu.gyroCal[2]),
            .tagMag = 0xFD,
            .magX = mpu.magnet[0],
            .magY = mpu.magnet[1],
            .magZ = mpu.magnet[2]
        };

        // send to receiver's DATA_PIPE_0 address
        //my_nrf.tx_destination((uint8_t[]){0x37,0x37,0x37,0x37,0x37});
        my_nrf.tx_destination(pipezero_addr);

        // time packet was sent
        time_sent = to_us_since_boot(get_absolute_time()); // time sent

        // send packet to receiver's DATA_PIPE_0 address
        success = my_nrf.send_packet(&payload_zero, sizeof(payload_zero));

        // time auto-acknowledge was received
        time_reply = to_us_since_boot(get_absolute_time()); // response time
        printf("Enviando paquete...");
        if (success)
        {
        printf("\nPacket sent:- Response: %lluμS | Payload: %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",time_reply - time_sent, 
        payload_zero.tagAcel, payload_zero.acelX, payload_zero.acelY, payload_zero.acelZ, payload_zero.tagGyro, payload_zero.gyroX, payload_zero.gyroY, payload_zero.gyroZ,
        payload_zero.tagMag, payload_zero.magX, payload_zero.magY, payload_zero.magZ);

        } else {

        printf("\nPacket not sent:- Receiver not available.\n");
        }

        sleep_ms(1000);
        // sprintf(str, "%d,%d,%d\n", mpu.acceleration[0], mpu.acceleration[1], mpu.acceleration[2]);
        // sd_writefile(ret, &fil_acel, str);

        // //Escribiendo datos del acelerometro
        // sprintf(str, "%d,%d,%d\n", mpu.acceleration[0], mpu.acceleration[1], mpu.acceleration[2]);
        // sd_writefile(ret, &fil_acel, str);

        // //Escribiendo datos del giroscopio
        // sprintf(str, "%d,%d,%d\n", mpu.gyro[0] - mpu.gyroCal[0], mpu.gyro[1] - mpu.gyroCal[1], mpu.gyro[2] - mpu.gyroCal[2]);
        // sd_writefile(ret, &fil_gyro, str);

        // //Escribiendo datos del magnetometro
        // sprintf(str, "%d,%d,%d\n", mpu.magnet[0], mpu.magnet[1], mpu.magnet[2]);
        // sd_writefile(ret, &fil_magnet, str);

        // //Escribiendo angulos
        // sprintf(str, "%d,%d\n", mpu.fullAngles[0], mpu.fullAngles[1]);
        // sd_writefile(ret, &fil_angles, str);

        if(gpio_get(interruptPin) == 1){ //Haciendo polling al botón de parar de guardar los datos (PENDIENTE POR INTERRUPCION)
            sys_stop();
        }  
    }
}

void sys_init(void){
    stdio_init_all();//Inicializar I/O de C
    printf("comienza inicialización del sistema\n");
    gpio_init(interruptPin);
    gpio_set_dir(interruptPin, GPIO_IN);

    //Delay
    sleep_ms(6000);
    //Inicializar todo el apartado de la SD
    // initialize_sd();
    // mount_drive(fr, &fs);
}

void sys_stop(){
    //sd_closefile(fr, &fil_acel);
    //sd_closefile(fr, &fil_gyro);
    //sd_closefile(fr, &fil_magnet);
    //sd_closefile(fr, &fil_angles);
    //gpio_put(PICO_DEFAULT_LED_PIN, 1);
    printf("Datos escritos");
    while (1);
}

