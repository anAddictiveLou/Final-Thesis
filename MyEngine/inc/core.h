#ifndef _STATE_H
#define _STATE_H

#include <iostream>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "hidapi.h"
#include "byte.h"

#define ME_UNKNOWN -2
#define ME_SUCCESS 0
#define ME_FAILURE -1
#define ME_SIZE 1024
#define VID 0x0483
#define PID 0x5762

#if defined(_WIN32)
    #include <Windows.h>
    #define AES_CMD "MyEngine/aes-win/AESCrypt_console_v310_x64/aescrypt.exe"
    #define SHOW_ERROR_MESSAGE(message) MessageBox(NULL, TEXT(message), TEXT("Error"), MB_OK | MB_ICONERROR)
#else
    #define AES_CMD "aescrypt"
    #define SHOW_ERROR_MESSAGE(message) system("zenity --error --text='" message "'")
#endif

// Enum definition
enum States
{
    INITILIZE,
    INTERGRITY_VERIFY,
    MUTAL_AUTHENCATION,
    KEY_EXCHANGE,
    DECRYPT,
    START_APP,
    APP_RUNNING,
    ABNORMAL_BEHAVIOR,
    FINISH
};

enum IntergrityVerify
{
    INTERGRITY_VERIFY_START,
    INTERGRITY_VERIFY_FINISH
};


int32_t runMyEngine(const char* parainputFileAES);

// State functions
int32_t initHandle();
int32_t intergrityVerifyHandle();
int32_t mutualAuthenticationHandle();
int32_t keyExchangeHandle();
int32_t decryptHandle(char* fileAES, char* password);
int32_t startAppHandle();
int32_t appRunningHandle();
int32_t abnormalBehaviorHandle();

#endif