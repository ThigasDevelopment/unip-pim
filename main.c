#include "config.h"
#include "menu.h"

#include <conio.h>

#include <stdio.h>
#include <stdlib.h>

#include <locale.h>
#include <unistd.h>

int main () {
    system (CLEAR_COMMAND);
    setlocale (LC_ALL, "");

    while (MENU_RUNNING) {
        int current = getMenu ();
        displayMenu (current);
    }

    printf ("\nSaindo do Hortifruti, volte sempre !");

    sleep (TIME_TO_EXIT);
    system ("exit");

    return 0;
}