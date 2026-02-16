#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void progress_bar(const char *text, double duration_sec) {
    int width = 30;
    double step_time = duration_sec / width;

    for (int i = 0; i <= width; ) {
        int jump = 1 + rand() % 3; // рандомный «шаг» 1–3 блока
        if (i + jump > width) jump = width - i;

        printf("\r%s [", text);
        for (int j = 0; j < i + jump; j++) printf("█");
        for (int j = i + jump; j < width; j++) printf(" ");
        if (i + jump == width)
            printf("]    Done!");
        else
            printf("]    ");
        fflush(stdout);

        i += jump;
        usleep((int)(step_time * 1000000 * jump));
    }

    printf("\n");
}

int main() {
    srand(time(NULL));
    system("clear");

    // случайная задержка перед каждым прогрессом (имитация "внезапного апдейта")
    sleep(rand() % 5 + 1);
    progress_bar("Getting new update", 10.0);

    sleep(rand() % 5 + 1);
    progress_bar("Installing update", 20.0);

    sleep(rand() % 5 + 1);
    progress_bar("Finalizing update", 5.0);

    printf("Your PC will restart in ");
    for (int i = 5; i > 0; i--) {
        printf("%d...", i);
        fflush(stdout);
        sleep(1);
    }
    printf("\n");

    // вызов твоей команды shutdown
    system("/bin/shutdown -r");  
    return 0;
}
