#include "core.h"

#define MAX_STR 1024  // for manufacturer, product strings
#define MAX_BUF 1024  // for buf reads & writes


int main(int argc, char **argv)
{
    int ret = ME_FAILURE;
    if (argc != 2)
    {
        printf("Usage: %s <inputFileAES>\n", argv[0]);
        return ret;
    }
    char* inputFileAES = argv[1];
    
    // Check if the file extension is ".aes"
    if (strstr(inputFileAES, ".aes") == NULL)
    {
        SHOW_ERROR_MESSAGE("The file extension must be \".aes\"");
        return ret;
    }
    
    ret = runMyEngine(inputFileAES);
    return ret;
}