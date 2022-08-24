#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"
#include "sd_manager.h"

void initialize_sd(){
    if (!sd_init_driver()) {
        printf("No se pudo iniciar la tarjeta SD\r\n");
        while (true);
    }
}

void mount_drive(FRESULT fr, FATFS* fs){
    fr = f_mount(fs, "0:", 1);
    if (fr != FR_OK) {
        printf("No se pudo montar el sistema de archivos. ERROR: (%d)\r\n", fr);
        while (true);
    }
}

void sd_openfileW(FRESULT fr, FIL* fil, const TCHAR* filename){
    fr = f_open(fil, filename, FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK) {
        printf("No se puede abrir el archivo. ERROR: (%d)\r\n", fr);
        while (true);
    }
}

void sd_writefile(int ret, FIL* fil, const TCHAR* msg){
    ret = f_printf(fil, msg);
    if (ret < 0) {
        printf("No se pudo escribir en el archivo. ERROR: (%d)\r\n", ret);
        f_close(fil);
        while (true);
    }
}

void sd_closefile(FRESULT fr, FIL* fil){
    fr = f_close(fil);
    if (fr != FR_OK) {
        printf("No se puede cerrar el archivo. ERROR: (%d)\r\n", fr);
        while (true);
    }
}

void sd_openfileR(FRESULT fr, FIL* fil, const TCHAR* filename){
    fr = f_open(fil, filename, FA_READ);
    if (fr != FR_OK) {
        printf("No se puede abrir el archivo en lectura. ERROR: (%d)\r\n", fr);
        while (true);
    }
}

void sd_printfile(TCHAR* buff, FIL* fil){
    printf("Leyendo desde un archivo...:\r\n");
    printf("---\r\n");
    while (f_gets(buff, sizeof(buff), fil)) {
        printf(buff);
    }
    printf("\r\n---\r\n");
}