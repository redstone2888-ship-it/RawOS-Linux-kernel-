#ifndef UI_FUNCS_H
#define UI_FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>


/* ========= RAW MODE ========= */

static struct termios orig_term;

static void ui_raw_on() {
    tcgetattr(STDIN_FILENO, &orig_term);
    struct termios raw = orig_term;

    raw.c_lflag &= ~(ECHO | ICANON);
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

static void ui_raw_off() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_term);
}


/* ========= SCREEN ========= */

static void ui_clear() {
    printf("\033[2J\033[H");
}

static void ui_move(int y, int x) {
    printf("\033[%d;%dH", y+1, x+1);
}

static void ui_hide_cursor() {
    printf("\033[?25l");
}

static void ui_show_cursor() {
    printf("\033[?25h");
}

static void ui_refresh() {
    fflush(stdout);
}


/* ========= COLORS / STYLE ========= */

static void ui_reset() {
    printf("\033[0m");
}

static void ui_reverse_on() {
    printf("\033[7m");
}

static void ui_reverse_off() {
    printf("\033[27m");
}

static void ui_color(int fg) {
    printf("\033[3%dm", fg); // 0-7
}


/* ========= PRINT ========= */

static void ui_print(int y, int x, const char *s) {
    ui_move(y,x);
    printf("%s", s);
}

/* ========= INPUT ========= */

enum {
    UI_KEY_NONE = 0,
    UI_KEY_UP,
    UI_KEY_DOWN,
    UI_KEY_LEFT,
    UI_KEY_RIGHT,
    UI_KEY_ENTER
};

static int ui_getkey() {
    char c = getchar();

    if(c == '\n') return UI_KEY_ENTER;

    if(c == '\033') {
        getchar(); // [
        char d = getchar();

        if(d=='A') return UI_KEY_UP;
        if(d=='B') return UI_KEY_DOWN;
        if(d=='C') return UI_KEY_RIGHT;
        if(d=='D') return UI_KEY_LEFT;
    }

    return UI_KEY_NONE;
}

static void ui_get_size(int *rows, int *cols) {
    struct winsize ws;

    int fd = STDOUT_FILENO;
    if (!isatty(fd)) { // не tty
        *rows = 24;
        *cols = 80;
        return;
    }

    if(ioctl(fd, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0 || ws.ws_row == 0) {
        *rows = 24;
        *cols = 80;
        return;
    }

    *rows = ws.ws_row;
    *cols = ws.ws_col;
}

/* ========= CENTER PRINT ========= */

static void ui_print_center_xy(int y, int x, const char *s) {
    ui_move(y, x);
    printf("%s", s);
}

static void ui_print_center_h(int y, const char *s) {
    int len = strlen(s);
    int cols;
    int rows;
    ui_get_size(&rows, &cols);
    int x = (cols - len) / 2;
    ui_print_center_xy(y, x, s);
}

static void ui_print_center(int y, const char *s) {
    int rows, cols;
    ui_get_size(&rows, &cols);

    int len = strlen(s);
    int x = (cols - len) / 2;
    if(x < 0) x = 0;

    ui_print(y, x, s);
}

/* ========= MENU ========= */

static int ui_menu_select(char *choices[], int n) {
    int highlight = 0;

    ui_raw_on();
    ui_hide_cursor();

    while(1) {
        ui_clear();

        for(int i=0;i<n;i++) {
            if(i==highlight) ui_reverse_on();
            ui_print(i, 2, choices[i]);
            ui_reset();
        }

        ui_refresh();

        int key = ui_getkey();

        if(key == UI_KEY_UP && highlight>0) highlight--;
        else if(key == UI_KEY_DOWN && highlight<n-1) highlight++;
        else if(key == UI_KEY_ENTER) break;
    }

    ui_show_cursor();
    ui_raw_off();

    return highlight;
}


/* ========= MSG BOX ========= */

static void ui_msgbox(const char *text) {
    ui_raw_on();
    ui_hide_cursor();
    ui_clear();

    ui_print_center(10, text);
    ui_print_center(12, "[ press ENTER ]");

    ui_refresh();

    while(ui_getkey()!=UI_KEY_ENTER);

    ui_show_cursor();
    ui_raw_off();
}

static void ui_msgbox_multiline(const char *text) {
    ui_raw_on();
    ui_hide_cursor();
    ui_clear();

    int rows, cols;
    ui_get_size(&rows, &cols);

    char buf[2048];
    snprintf(buf, sizeof(buf), "%s", text);

    // считаем строки
    int count = 0;
    for(char *p = buf; *p; p++)
        if(*p == '\n') count++;
    count++; // последняя строка

    int start_y = (rows - count) / 2;

    char *line = strtok(buf, "\n");
    int y = start_y;

    while(line) {
        int len = strlen(line);
        int x = (cols - len) / 2;
        if(x < 0) x = 0;

        ui_print(y, x, line);
        y++;
        line = strtok(NULL, "\n");
    }

    ui_print_center(y + 1, "[ press ENTER ]");
    ui_refresh();

    while(ui_getkey() != UI_KEY_ENTER);

    ui_show_cursor();
    ui_raw_off();
}

/* ========= CENTERED MENU (safe) ========= */

static int ui_menu_select_center(char *choices[], int n) {
    int highlight = 0;

    ui_raw_on();
    ui_hide_cursor();

    int rows = 24, cols = 80;
    ui_get_size(&rows, &cols);

    int start_y = (rows - n) / 2;
    if(start_y < 0) start_y = 0;

    while(1) {
        ui_clear();

        for(int i = 0; i < n; i++) {
            int len = strlen(choices[i]);

            int x = (cols - len) / 2;
            if(x < 0) x = 0;

            if(i == highlight)
                ui_reverse_on();

            ui_print(start_y + i, x, choices[i]);

            ui_reset();
        }

        ui_refresh();

        int key = ui_getkey();

        if(key == UI_KEY_UP && highlight > 0) highlight--;
        else if(key == UI_KEY_DOWN && highlight < n-1) highlight++;
        else if(key == UI_KEY_ENTER) break;
    }

    ui_show_cursor();
    ui_raw_off();

    return highlight;
}


#endif
