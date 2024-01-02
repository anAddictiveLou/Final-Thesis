#ifndef _BYTE_H
#define _BYTE_H
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int str2buf(void *buffer, char *delim_str, char *string, int buflen, int bufelem_size);

#endif