#include <stdio.h>
#include "pico/stdlib.h"
#include "sd_card.h"
#include "ff.h"

void initialize_sd(void);
void mount_drive(FRESULT fr, FATFS* fs);
void sd_openfileW(FRESULT fr, FIL* fil, const TCHAR* filename);
void sd_writefile(int ret, FIL* fil, const TCHAR* msg);
void sd_closefile(FRESULT fr, FIL* fil);
void sd_openfileR(FRESULT fr, FIL* fil, const TCHAR* filename);
void sd_printfile(TCHAR* buff, FIL* fil);
