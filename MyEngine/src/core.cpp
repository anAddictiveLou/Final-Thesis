#include "core.h"

enum States state;
int ret;
char gOutFile[ME_SIZE] = {0};
pid_t cPid;
hid_device* device;


void sigchld_handler(int signo) {
    (void)signo;
    int status;
    pid_t pid;

    // while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    //     if (WIFEXITED(status)) {
    //         // Child process exited normally
    //         // printf("Child process %d exited with status %d\n", pid, WEXITSTATUS(status));
    //     } else if (WIFSIGNALED(status)) {
    //         // Child process terminated by a signal
    //         // printf("Child process %d terminated by signal %d\n", pid, WTERMSIG(status));
    //     }
    //     // You can add more handling for other cases if needed
    // }
    wait(NULL);
    SHOW_ERROR_MESSAGE("MyEngine is forced to terminated");
    exit(EXIT_SUCCESS);
}

// Function definitions
int32_t runMyEngine(const char* parainputFileAES)
{
    int ret = ME_FAILURE;
    state = INITILIZE;
    char inputFileAES[ME_SIZE] = {0};
    char outputFile[ME_SIZE] = {0};
    char password[ME_SIZE];

    // char* pidvidStr = "0483/5762";
    // if( sscanf(pidvidStr, "%4hx/%4hx", &vid,&pid) !=2 ) {  // match "23FE/AB12"
    //     if( !sscanf(pidvidStr, "%4hx:%4hx", &vid,&pid) ) { // match "23FE:AB12"
    //         // else try parsing standard dec/hex values
    //         int wordbuf[4]; // a little extra space
    //         int parsedlen = str2buf(wordbuf, ":/, ", optarg, sizeof(wordbuf), 2);
    //         vid = wordbuf[0]; pid = wordbuf[1];
    //     }
    // }

    // for test decrypt state
    state = DECRYPT;
    strcpy(password, "abcde");

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
            ret = decryptHandle(inputFileAES, password);
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
    device = hid_open((unsigned short) VID, (unsigned short) PID, NULL);
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
    int ret = ME_FAILURE;
    memset(gOutFile, 0, ME_SIZE);
    strncpy(gOutFile, fileAES, ME_SIZE);
    gOutFile[strlen(gOutFile) - 4] = '\0';
    if (NULL == fileAES || NULL == password)
        return ret;
    char cmd[ME_SIZE] = {0};
    snprintf(cmd, sizeof(cmd), "%s -d -p %s %s", AES_CMD, password, fileAES);
    FILE *pipe = popen(cmd, "r");

    if (pipe == NULL) {
        perror("popen");
        return ret;
    }
    // Read the output of the command
    // char buffer[ME_SIZE];
    // while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
    //     printf("%s", buffer);
    // }
    pclose(pipe);
    
    if (access(gOutFile, F_OK) == -1) {
        SHOW_ERROR_MESSAGE("Decrypt failure!");
        return ret;
    }
    memset(cmd, 0, ME_SIZE);
    snprintf(cmd, ME_SIZE, "chmod 777 %s", gOutFile);
    system(cmd);
    ret =  ME_SUCCESS;
    return ret;
}

int32_t startAppHandle()
{
    int ret = ME_FAILURE;
    cPid = fork();

    if (cPid == -1) {
        return ret;
    } else if (cPid == 0) {
        printf("%s", gOutFile);
        if (execl(gOutFile, gOutFile, NULL) == -1) {
            SHOW_ERROR_MESSAGE("Start app failure!");
            exit(ME_FAILURE);
        }
    } else {
        /* parent process */
        struct sigaction sa;
        sa.sa_handler = sigchld_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART | SA_NOCLDSTOP;
        if (sigaction(SIGCHLD, &sa, NULL) == -1) {
            perror("sigaction");
            return ret;
        }
        // Continue parent process

    }
    ret = ME_SUCCESS;
    return ret;
}

int32_t appRunningHandle()
{
    int ret = ME_FAILURE;
    if (device) {
        hid_close(device);
        device = NULL;
    }
    while (1)
    {
        /* is Usb still plugged in or not */
        device = hid_open((unsigned short) VID, (unsigned short) PID, NULL);
        if (!device) {
            ret = abnormalBehaviorHandle();
            SHOW_ERROR_MESSAGE("usb device is plugged out!");
            return ret;
        } else {
            hid_close(device);
        }
        device = NULL;
    }

    ret = ME_UNKNOWN;
    return ret;
}

int32_t abnormalBehaviorHandle()
{
    int ret = ME_FAILURE;
    printf("Child pid: %d", cPid);
    if (cPid != 0)
    {
        // Send SIGTERM signal to the specified process
        if (kill(cPid, SIGTERM) == 0) {
            printf("Process %d terminated successfully with SIGTERM.\n", cPid);
        } else {
            perror("Error terminating process");
            // If the process cannot be terminated with SIGTERM, you might try SIGKILL
            if (kill(cPid, SIGKILL) == 0) {
                printf("Process %d terminated forcefully with SIGKILL.\n", cPid);
            } else {
                perror("Error forcefully terminating process");
            }
        }
        cPid = 0;
        if (access(gOutFile, F_OK) != -1) {
            remove(gOutFile);
        }
    }

    return ret;
}
