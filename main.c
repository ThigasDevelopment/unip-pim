#include "config.h"

#include <conio.h>

#include <stdio.h>
#include <stdlib.h>

#include <locale.h>
#include <unistd.h>

int MENU_WINDOW = 0;
int MENU_RUNNING = 1;

int getMenu () {
    return MENU_WINDOW;
}

int setMenu (int new) {
    const int old = getMenu ();

    if (old != new) {
        MENU_WINDOW = new;

        system (CLEAR_COMMAND);

        return 0;
    }

    return 1;
}

int display (int menu) {
    if (menu < 0 || menu > 2) return 1;

    int choice;
    choice = 0;

    printf ("Hortifruti %s\n\n", HORTIFRUIT_NAME);

    if (menu == 0) {
        char *options[] = {
            "Entrar como: Administrador",
            "Entrar como: Cliente",
            "Sair"
        };

        int options_size = sizeof (options) / sizeof (char *);

        for (int i = 0; i < options_size; i++) {
            printf ("[%d] = %s\n", (i + 1), options[i]);
        }

        printf ("\nEscolha sua opcao: ");
        scanf ("%d", &choice);
        getchar ();

        if (choice != options_size) {
            setMenu (choice);
        } else {
            MENU_RUNNING = 0;
        }
    } else if (menu == 1) {
        
    }

    return 0;
}

int main () {
    system (CLEAR_COMMAND);
    setlocale (LC_ALL, "");

    while (MENU_RUNNING) {
        int current = getMenu ();
        display (current);
    }

    printf ("\nSaindo do Hortifruti, volte sempre !");

    sleep (TIME_TO_EXIT);
    system ("exit");

    return 0;
}