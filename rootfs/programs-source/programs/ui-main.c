#include "ui-funcs.h"

void shutdown_menu() {
    char *choices[] = {
        "Reboot",
        "Power off",
        "Back"
    };
    int choice = ui_menu_select(choices, 3);

    switch (choice)
    {
    case 0:
        system("/bin/shutdown -r");
        break;
    case 1:
        system("/bin/shutdown");
        break;
    default:
        // ничего не делаем, просто возвращаемся в главное меню
        break;
    }
}

void settings_menu() {
    char *choices[] = {
        "About...",
        "Back"
    };
    int choice = ui_menu_select(choices, 2);

    switch (choice)
    {
    case 0:
        ui_msgbox_multiline(
            "RAWOS v1.2\n"
            "Copyright (c) 2026, RAWOS Team. All rights reserved.\n"
            "\n"
            "Whats new in v1.2:\n"
            "- Improved UI\n"
            "- Better error handling\n"
            "- Fixed bugs in shutdown menu\n"
        );
        break;
    default:
        // ничего не делаем, просто возвращаемся в главное меню
        break;
    }
}

void maybe_run_update() {
    // шанс примерно 1 из 2
    if ((rand() % 100) < 50) {  // 50% шанс
        system("update");
    }
}

int main() {
    maybe_run_update();
    int running = 1;
    while (running)
    {
        char *choices[] = {
            "Programs",
            "Settings (not working yet)",
            "Power off",
            "Back to terminal"
        };
        int choice = ui_menu_select_center(choices, 4);

        switch (choice)
        {
        case 0:
            ui_msgbox("Sorry, but there are no programs yet :(");
            break;
        case 1:
            settings_menu();
            break;
        case 2:
            shutdown_menu();
            break;
        default:
            printf("Exiting to terminal...\n");
            running = 0;
            // для гигачадов которые не юзают интерфейс
            break;
        }
    }

    ui_clear();
    return 0;
}