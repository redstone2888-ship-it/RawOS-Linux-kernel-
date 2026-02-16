#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/reboot.h>

void do_reboot(int cmd) {
    kill(-1, SIGTERM);
    sleep(3);

    kill(-1, SIGKILL);
    sleep(1);

    sync();

    reboot(cmd);
}

void do_forced(int cmd) {
    reboot(cmd);
}

int main(int argc, char *argv[]) {
    int force = 0;
    int reboot_flag = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0) force = 1;
        else if (strcmp(argv[i], "-r") == 0) reboot_flag = 1;
    }

    int cmd = reboot_flag ?
        LINUX_REBOOT_CMD_RESTART :
        LINUX_REBOOT_CMD_POWER_OFF;

    if (force)
        do_forced(cmd);
    else
        do_reboot(cmd);

    return 0;
}
