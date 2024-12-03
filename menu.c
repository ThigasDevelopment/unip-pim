#include "file.h"
#include "menu.h"

#include "cJSON.h"
#include "users.h"

#include "hortifruit.h"

#include "config.h"

#include <time.h>
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

int isValidDate (const char* date) {
    struct tm tm;
    memset (&tm, 0, sizeof (struct tm));
    
    // Formato da data: "dd/mm/yyyy"
    if (sscanf (date, "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) != 3) {
        return 0; // Formato inválido
    }

    // Ajuste dos valores da estrutura tm
    tm.tm_mon -= 1;         // Meses vão de 0 a 11
    tm.tm_year -= 1900;     // Anos desde 1900
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;       // Detecta automaticamente horário de verão

    // Usa mktime para validar a data
    if (mktime(&tm) == -1) {
        return 0; // Data inválida
    }

    return 1; // Data válida
}

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
                { 5, "Buscar" },
                { 6, "Estoque" },
                { 7, "Registrar" },
                { 8, "Deletar" },

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

            if (withdraw < 1) {
                printf ("\nVoce nao pode sacar R$0.00.");

                sleep (TIME_TO_ACTION);
                setMenu (1);

                return 1;
            }

            if (withdraw > balance) {
                printf ("\nO caixa nao possui esse valor.");

                sleep (TIME_TO_ACTION);
                setMenu (1);

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

            if (deposit < 1) {
                printf ("\nVoce nao pode depositar R$0.00.");

                sleep (TIME_TO_ACTION);
                setMenu (1);

                return 1;
            }

            setBalance (balance + deposit);

            printf ("\nVoce depositou R$%d,00 no caixa.", deposit);

            sleep (TIME_TO_ACTION);
            setMenu (1);
        } else if (menu == 5) {
            char name[50];
            printf ("Informe o nome do produto: ");
            fgets (name, 50, stdin);
            name[strcspn (name, "\n")] = '\0';

            char supplier[50];
            printf ("Informe o fornecedor do produto: ");
            fgets (supplier, 50, stdin);
            supplier[strcspn (supplier, "\n")] = '\0';

            int index = getProduct (name, supplier);

            if (index == -1) {
                printf ("\nO produto [ %s ] nao foi encontrado.", name);
                sleep (TIME_TO_ACTION);
                setMenu (2);

                return 1;
            }

            int products_size;
            Product* products = getProducts (&products_size);

            printf ("\nInformacoes do produto:\n");

            printf ("\nNome: %s", products[index].name);
            printf ("\nQuantidade: %d", products[index].amount);
            printf ("\nPreco: %d.00", products[index].price);
            printf ("\nFornecedor: %s", products[index].supplier);
            printf ("\nData de validade: %s\n", products[index].expiration_date);

            printf ("\nPressione qualquer tecla para voltar.");
            getch ();

            setMenu (2);
        } else if (menu == 6) {
            int products_size;
            Product* products = getProducts (&products_size);

            if (products_size < 1) {
                printf ("Nao ha nenhum produto cadastrado no sistema, pressione qualquer tecla para voltar.");
                getch ();

                setMenu (2);

                return 1;
            }

            printf ("Produtos:\n");

            for (int i = 0; i < products_size; i++) {
                Product data = products[i];

                printf ("[%d] = Nome: %s, Quantidade: %d, Preco: %d.00, Fornecedor: %s, Data de validade: %s\n", (i + 1), data.name, data.amount, data.price, data.supplier, data.expiration_date);
            }

            printf ("\nPressione qualquer tecla para voltar.");
            getch ();

            setMenu (2);
        } else if (menu == 7) {
            char name[50];
            printf ("Informe o nome do produto: ");
            fgets (name, 50, stdin);
            name[strcspn (name, "\n")] = '\0';

            int price;
            printf ("Informe o preco do produto: R$");
            scanf ("%d", &price);

            int amount;
            printf ("Informe a quantidade inicial do produto: ");
            scanf ("%d", &amount);
            getchar ();

            char supplier[50];
            printf ("Informe o fornecedor do produto: ");
            fgets (supplier, 50, stdin);
            supplier[strcspn (supplier, "\n")] = '\0';

            char expiration_date[50];
            printf ("Informe a data de validade do produto: ");
            fgets (expiration_date, 50, stdin);
            expiration_date[strcspn (expiration_date, "\n")] = '\0';

            if (isValidDate (expiration_date) != 1) {
                printf ("\nFormato da data de validade invalido (DD/MM/AAAA).");

                sleep (TIME_TO_ACTION);
                setMenu (2);

                return 1;
            }

            if (createProduct (name, amount, price, supplier, expiration_date) == 0) {
                printf ("\nO produto [ %s ] foi criado com sucesso.", name);
            } else {
                printf ("\nOcorreu um erro ao criar o produto, contate um administrador.");
            }

            sleep (TIME_TO_ACTION);
            setMenu (2);
        } else if (menu == 8) {
            char name[50];
            printf ("Informe o nome do produto: ");
            fgets (name, 50, stdin);
            name[strcspn (name, "\n")] = '\0';

            char supplier[50];
            printf ("Informe o fornecedor do produto: ");
            fgets (supplier, 50, stdin);
            supplier[strcspn (supplier, "\n")] = '\0';

            if (deleteProduct (name, supplier) == 0) {
                printf ("\nO produto [ %s ] foi deletado com sucesso.", name);
            } else {
                printf ("\nOcorreu um erro ao deletar o produto, confirme as informacoes.");
            }

            sleep (TIME_TO_ACTION);
            setMenu (2);
        } else {
            printf ("\nOpcao invalida.");

            sleep (TIME_TO_ACTION);
            setMenu (0);
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
        } else {
            printf ("\nOpcao invalida.");

            sleep (TIME_TO_ACTION);
            setMenu (0);
        }
    }

    return 0;
}