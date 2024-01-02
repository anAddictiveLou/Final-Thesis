#include "core.h"

extern enum States state;

// Function definitions
int32_t runMyEngine(const char* parainputFileAES)
{
    int ret = ME_FAILURE;
    char inputFileAES[ME_SIZE];

    strcpy(inputFileAES, parainputFileAES);

    while (state != FINISH)
    {
        switch (state)
        {
        case INITILIZE:
            ret = initHandle();
            if (ret == ME_SUCCESS)
            {
                state = INTERGRITY_VERIFY;
            }
            else
            {
                state = FINISH;
            }
            break;
        case INTERGRITY_VERIFY:
            ret = intergrityVerifyHandle();
            if (ret == ME_SUCCESS)
            {
                state = MUTAL_AUTHENCATION;
            }
            else
            {
                state = FINISH;
            }
            break;
        case MUTAL_AUTHENCATION:
            ret = mutualAuthenticationHandle();
            if (ret == ME_SUCCESS)
            {
                state = KEY_EXCHANGE;
            }
            else
            {
                state = FINISH;
            }
            break;
        case KEY_EXCHANGE:
            ret = keyExchangeHandle();
            if (ret == ME_SUCCESS)
            {
                state = DECRYPT;
            }
            else
            {
                state = FINISH;
            }
            break;
        case DECRYPT:
            ret = decryptHandle(inputFileAES);
            if (ret == ME_SUCCESS)
            {
                state = START_APP;
            }
            else
            {
                state = FINISH;
            }
            break;
        case START_APP:
            ret = startAppHandle();
            if (ret == ME_SUCCESS)
            {
                state = APP_RUNNING;
            }
            else
            {
                state = FINISH;
            }
            break;
        case APP_RUNNING:
            ret = appRunningHandle();
            if (ret == ME_FAILURE)
            {
                state = ABNORMAL_BEHAVIOR;
            }
            else if (ret == ME_UNKNOWN)
            {
                state = FINISH;
            }
            break;
        case ABNORMAL_BEHAVIOR:
            ret = abnormalBehaviorHandle();
            state = FINISH;
            break;
        default:
            state = FINISH;
            ret = ME_UNKNOWN;
            break;
        }
    }
    return ret;
}

int32_t initHandle()
{
    int ret = ME_FAILURE;
    // Initialize the HIDAPI library
    if (hid_init() != 0) {
        SHOW_ERROR_MESSAGE("hid_init failed");
        return ret;
    }

    // Open the device using the VID (Vendor ID) and PID (Product ID)
    hid_device* device = hid_open(0x0483, 0x5762, NULL);
    if (!device) {
        SHOW_ERROR_MESSAGE("no device found");
        return ret;
    }
    ret = ME_SUCCESS;
    return ret;
}

int32_t intergrityVerifyHandle()
{
    int ret = ME_FAILURE;
    return ret;
}

int32_t mutualAuthenticationHandle()
{
    int ret = ME_FAILURE;
    return ret;
}

int32_t keyExchangeHandle()
{
    int ret = ME_FAILURE;
    return ret;
}

int32_t decryptHandle(char* fileAES, char* password)
{
    // aescrypt -d -p [password] [fileAES]
    int ret = ME_SUCCESS;
    char cmd[ME_SIZE] = {0};
    snprintf(cmd, sizeof(cmd), "%s -d -p %s %s", AES_CMD, password, fileAES);
    return ret;
}

int32_t startAppHandle()
{
    int ret = ME_FAILURE;
    return ret;
}

int32_t appRunningHandle()
{
    int ret = ME_FAILURE;
    hid_device* device = NULL;
    while (1)
    {
        device = hid_open(0x0483, 0x5762, NULL);
        if (!device) {
            SHOW_ERROR_MESSAGE("usb device is not connected");
            return ret;
        }
    }
    ret = ME_UNKNOWN;
    return ret;
}

int32_t abnormalBehaviorHandle()
{
    int ret = ME_FAILURE;
    return ret;
}