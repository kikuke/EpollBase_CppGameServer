#include <string.h>
#include <sys/stat.h>

#include "spfileio.h"
void mkdir_all(const char* dir_path)
{
    char buff[SPFILEIO_BUFF];
    char *p_dir = buff;

    strcpy(buff, dir_path);
    buff[SPFILEIO_BUFF-1] = '\0';

    while(*p_dir){
        if(*p_dir == '/'){
            *p_dir = '\0';
            mkdir(buff, 0777);
            *p_dir = '/';
        }
        p_dir++;
    }
    mkdir(buff, 0777);
}