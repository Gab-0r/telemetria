#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "sd_manager.h"

int main() {

    FRESULT fr;
    FATFS fs;
    FIL fil;
    int ret;
    char buf[100];
    char filename[] = "sdManager_test.txt";

    //Inicializar seriales
    stdio_init_all();

    sleep_ms(5000);

    // Inicializar tarjeta SD
    initialize_sd();

    //Montar el volumen
    mount_drive(fr, &fs);

    //Abrir archivo para escribir
    sd_openfileW(fr, &fil, filename);

    //Escribiendo en el archivo
    sd_writefile(ret, &fil, "PRUEBITAAAAAAA");

    //Cerrar el archivo
    sd_closefile(fr, &fil);

    //Abrir el archivo en modo lectura
    sd_openfileR(fr, &fil, filename);

    //Imprimir en serial lo que se lee del archivo
    sd_printfile(buf, &fil);

    //Cerrar el archivo
    sd_closefile(fr, &fil);

    //Desmontar dispositivo
    f_unmount("0:");

    // Loop forever doing nothing
    while (true) {
        sleep_ms(1000);
    }
}