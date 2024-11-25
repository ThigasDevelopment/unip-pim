#include "file.h"
#include "menu.h"

#include "cJSON.h"
#include "users.h"

#include "hortifruit.h"

#include "config.h"

#include <conio.h>
#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

#include <string.h>

int MENU_WINDOW = 0;
int MENU_RUNNING = 1;

typedef struct {
    int id;

    char name[50];
} Options;

int getMenu () {
    return MENU_WINDOW;
}

int setMenu (int menu) {
    const int old = getMenu ();

    if (old != menu) {
        MENU_WINDOW = menu;

        system (CLEAR_COMMAND);

        return 0;
    }

    return 1;
}

int displayMenu (int menu) {
    if (menu < 0) return 1;

    int choice;
    choice = 0;

    printf ("Hortifruti %s\n\n", HORTIFRUIT_NAME);

    if (strlen (auth.username) > 0) {
        printf ("Bem - vindo(a) %s!", auth.username);
        printf ("\nConexao: Administrador\n\n");
        
        if (menu == 0) {
            Options options[] = {
                { 1, "Caixa" },
                { 2, "Produtos" },

                { 999, "Sair" }
            };
            
            int options_size = sizeof (options) / sizeof (options[0]);

            for (int i = 0; i < options_size; i++) {
                printf ("[%d] = %s\n", (i + 1), options[i].name);
            }

            printf ("\nEscolha sua opcao: ");
            scanf ("%d", &choice);
            getchar ();

            int realID = options[(choice - 1)].id;

            if (realID != 999) {
                setMenu (realID);
            } else {
                MENU_RUNNING = 0;
            }
        } else if (menu == 1) {
            int balance = getBalance ();
            printf ("Valor no caixa: %d,00\n\n", balance);

            Options options[] = {
                { 3, "Sacar" },
                { 4, "Depositar" },

                { 0, "Voltar" }
            };
            
            int options_size = sizeof (options) / sizeof (options[0]);

            for (int i = 0; i < options_size; i++) {
                printf ("[%d] = %s\n", (i + 1), options[i].name);
            }

            printf ("\nEscolha sua opcao: ");
            scanf ("%d", &choice);
            getchar ();

            int realID = options[(choice - 1)].id;
            setMenu (realID);
        } else if (menu == 2) {
            Options options[] = {
                { 5, "Produtos" },
                { 6, "Registrar" },
                { 7, "Deletar" },

                { 0, "Voltar" }
            };

            int options_size = sizeof (options) / sizeof (options[0]);

            for (int i = 0; i < options_size; i++) {
                printf ("[%d] = %s\n", (i + 1), options[i].name);
            }

            printf ("\nEscolha sua opcao: ");
            scanf ("%d", &choice);
            getchar ();

            int realID = options[(choice - 1)].id;
            setMenu (realID);
        } else if (menu == 3) {
            int balance = getBalance ();
            printf ("Valor no caixa: %d,00\n\n", balance);

            int withdraw;
            withdraw = 0;

            printf ("Informe um valor: R$");
            scanf ("%d", &withdraw);
            getchar ();

            if (withdraw > balance) {
                printf ("\nO caixa nao possui esse valor.");

                sleep (TIME_TO_ACTION);
                system (CLEAR_COMMAND);

                return 1;
            }

            setBalance (balance - withdraw);

            printf ("\nVoce sacou R$%d,00 do caixa.", withdraw);
            sleep (TIME_TO_ACTION);
            setMenu (1);
        } else if (menu == 4) {
            int balance = getBalance ();
            printf ("Valor no caixa: %d,00\n\n", balance);

            int deposit;
            deposit = 0;

            printf ("Informe um valor: R$");
            scanf ("%d", &deposit);
            getchar ();

            setBalance (balance + deposit);

            printf ("\nVoce depositou R$%d,00 no caixa.", deposit);
            sleep (TIME_TO_ACTION);
            setMenu (1);
        }
    } else {
        if (menu == 0) {
            Options options[] = {
                { 1, "Entrar" },
                { 999, "Sair" }
            };

            int options_size = sizeof (options) / sizeof (options[0]);

            for (int i = 0; i < options_size; i++) {
                printf ("[%d] = %s\n", (i + 1), options[i].name);
            }

            printf ("\nEscolha sua opcao: ");
            scanf ("%d", &choice);
            getchar ();

            int realID = options[(choice - 1)].id;

            if (realID != 999) {
                setMenu (realID);
            } else {
                MENU_RUNNING = 0;
            }
        } else if (menu == 1) {
            char username[25];
            printf ("Informe o login: ");
            fgets (username, 25, stdin);
            username[strcspn (username, "\n")] = '\0';

            char password[25];
            printf ("Informe a senha: ");
            fgets (password, 25, stdin);
            password[strcspn (password, "\n")] = '\0';

            char *content = readFile ();

            Users finded = findUser (content, username);
            free (content);

            if (strlen (finded.username) < 1) {
                printf ("\nUsuario nao encontrado, tente novamente.");

                sleep (TIME_TO_ACTION);
                system (CLEAR_COMMAND);

                return 1;
            }

            if (strcmp (finded.password, password) != 0) {
                printf ("\nSenha incorreta, tente novamente.");

                sleep (TIME_TO_ACTION);
                system (CLEAR_COMMAND);

                return 1;
            }

            printf ("\nBem - vindo(a) de volta (%s), autenticando dados... Aguarde.\n", username);

            sleep (TIME_TO_ACTION);

            if (finded.administrator != 1) {
                printf ("\nVoce nao pode entrar nessa conta agora, tente novamente mais tarde.");

                sleep (TIME_TO_ACTION);
                system (CLEAR_COMMAND);

                return 1;
            }

            strcpy (auth.username, finded.username);
            strcpy (auth.password, finded.password);
            auth.administrator = finded.administrator;

            setMenu (0);
        }
    }

    return 0;
}