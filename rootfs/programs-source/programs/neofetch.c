#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/utsname.h>

/* ====== Получение CPU ====== */
void get_cpu(char *buf, size_t size) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) { snprintf(buf, size, "Unknown CPU"); return; }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            char *p = strchr(line, ':');
            if (p) p++;
            while (*p == ' ' || *p == '\t') p++;
            strncpy(buf, p, size-1);
            buf[size-1] = 0;
            char *nl = strchr(buf, '\n');
            if (nl) *nl = 0;
            break;
        }
    }
    fclose(f);
}

/* ====== Получение RAM ====== */
void get_ram(char *buf, size_t size) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) { snprintf(buf, size, "Unknown RAM"); return; }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            long kb;
            if (sscanf(line + 9, "%ld", &kb) == 1) {
                snprintf(buf, size, "%ld MB", kb / 1024);
            } else snprintf(buf, size, "Unknown RAM");
            break;
        }
    }
    fclose(f);
}

/* ====== Получение Uptime ====== */
void get_uptime(char *buf, size_t size) {
    FILE *f = fopen("/proc/uptime", "r");
    if (!f) { snprintf(buf, size, "Unknown uptime"); return; }

    double up;
    if (fscanf(f, "%lf", &up) == 1) {
        int h = (int)up / 3600;
        int m = ((int)up % 3600) / 60;
        int s = (int)up % 60;
        snprintf(buf, size, "%02dh %02dm %02ds", h, m, s);
    } else snprintf(buf, size, "Unknown uptime");

    fclose(f);
}

/* ====== Печать логотипа и инфы в терминал ====== */
void show_system_info() {
    char cpu[128], ram[64], uptime[64];
    struct utsname uname_info;
    uname(&uname_info);

    get_cpu(cpu, sizeof(cpu));
    get_ram(ram, sizeof(ram));
    get_uptime(uptime, sizeof(uptime));

    printf(
" ██████╗  █████╗ ██╗    ██╗    OS: RawOS 1.2\n"
" ██╔══██╗██╔══██╗██║    ██║    Kernel: %s\n"
" ██████╔╝███████║██║ █╗ ██║    Host: %s\n"
" ██╔══██╗██╔══██║██║███╗██║    Init: /init\n"
" ██║  ██║██║  ██║╚███╔███╔╝    Shell: RSH (my own shell)\n"
" ╚═╝  ╚═╝╚═╝  ╚═╝ ╚══╝╚══╝     CPU: %s\n"
"                               RAM: %s\n"
"                               Uptime: %s\n",
           uname_info.release,
           uname_info.nodename,
           cpu,
           ram,
           uptime
    );
}

/* ====== MAIN ====== */
int main() {
    show_system_info();
    return 0;
}
