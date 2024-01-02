/**
 * hidapitester.c -- Demonstrate HIDAPI via commandline
 *
 * 2019, Tod E. Kurt / github.com/todbot
 *
 */

#include "core.h"


#define MAX_STR 1024  // for manufacturer, product strings
#define MAX_BUF 1024  // for buf reads & writes


enum States state = INITILIZE;
int ret;


int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s <inputFileAES>\n", argv[0]);
        return ME_FAILURE;
    }
    char* inputFileAES = argv[1];
    
    // Check if the file extension is ".aes"
    if (strstr(inputFileAES, ".aes") == NULL)
    {
        SHOW_ERROR_MESSAGE("The file extension must be \".aes\"");
        return ME_FAILURE;
    }
    
    ret = runMyEngine(inputFileAES);
    return ret;
}