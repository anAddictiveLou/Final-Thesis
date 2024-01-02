#include "byte.h"

int str2buf(void *buffer, char *delim_str, char *string, int buflen, int bufelem_size)
{
    char *s;
    int pos = 0;
    if (string == NULL)
        return -1;
    memset(buffer, 0, buflen); // bzero() not defined on Win32?
    while ((s = strtok(string, delim_str)) != NULL && pos < buflen)
    {
        string = NULL;
        switch (bufelem_size)
        {
        case 1:
            ((uint8_t *)buffer)[pos++] = (uint8_t)strtol(s, NULL, 0);
            break;
        case 2:
            ((int *)buffer)[pos++] = (int)strtol(s, NULL, 0);
            break;
        }
    }
    return pos;
}
