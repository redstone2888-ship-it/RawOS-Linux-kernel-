#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/reboot.h>
#include <sys/types.h>
#include <sys/wait.h>

void powerOffOrReboot(int rebootFlag) {
    if (rebootFlag) {
        printf("[DEBUG]: Kernel reboot command\n");
        if (reboot(RB_AUTOBOOT) != 0) {
            perror("reboot failed");
        }
    } else {
        printf("[DEBUG]: Kernel poweroff command\n");
        if (reboot(RB_POWER_OFF) != 0) {
            perror("poweroff failed");
        }
    }
}

int main(int argc, char *argv[]) {
    int force = 0;
    int rebootFlag = 0;

    // Простейший парсер флагов
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) force = 1;
        else if (strcmp(argv[i], "-r") == 0) rebootFlag = 1;
    }

    printf("[SHUTDOWN]: Shutdown started\n");

    if (force) {
        printf("[SHUTDOWN]: Forced shutdown\n");
        powerOffOrReboot(rebootFlag);
        return 0;
    } else if (rebootFlag) {
        printf("[SHUTDOWN]: Reboot requested\n");
        powerOffOrReboot(1);
        return 0;
    }

    // Плавное выключение
    printf("[SHUTDOWN]: SIGTERM signal to tasks\n");
    kill(-1, SIGTERM);

    printf("[SHUTDOWN]: Waiting 5 seconds for tasks to finish...\n");
    sleep(5);

    printf("[SHUTDOWN]: Syncing disks...\n");
    if (system("sync") != 0) {
        perror("[SHUTDOWN]: sync error");
    }

    printf("[SHUTDOWN]: Stopping services... (add your commands here)\n");

    printf("[DEBUG]: Sending shutdown command to kernel...\n");
    powerOffOrReboot(0);

    printf("[SHUTDOWN]: Poweroff successful\n");
    return 0;
}
