// import's system's
#include "cJSON.h"
#include "config.h"

#include <stdio.h>

#include <string.h>
#include <stdlib.h>

#include <unistd.h>

// struct's system's
typedef struct
{
    char name[25];
    char pass[25];
} User;

// method's system's
char* readFile (const char* filename)
{
    FILE *file = fopen (filename, "r");

    fseek (file, 0, SEEK_END);
    long size = ftell (file);
    fseek (file, 0, SEEK_SET);

    char *content = malloc (size + 1);
    fread (content, 1, size, file);
    content[size] = '\0';

    fclose (file);

    return content;
}

User findUser (const char *array, const char *search)
{
    User new_user = { "", "" };

    cJSON *users = cJSON_Parse (array);

    if (users == NULL)
    {
        return new_user;
    }

    int size = cJSON_GetArraySize (users);

    for (int i = 0; i < size; i++)
    {
        cJSON *item = cJSON_GetArrayItem (users, i);

        cJSON *name = cJSON_GetObjectItem (item, "name");
        cJSON *pass = cJSON_GetObjectItem (item, "pass");

        if (strcmp (name->valuestring, search) == 0)
        {
            strcpy (new_user.name, name->valuestring);
            strcpy (new_user.pass, pass->valuestring);

            break;
        }
    }

    cJSON_Delete (users);

    return new_user;
}

int main () {
    system (CLEAR_COMMAND);

    int option;
    option = 0;

    while (option < 1 || option > 3)
    {
        printf ("Bem - vindo(a) ao Hortifruti %s, escolha uma opcao abaixo:\n\n", HORTIFRUIT_NAME);

        printf ("[1] Entrar como: Administrador.\n");
        printf ("[2] Entrar como: Cliente.\n");

        printf ("[3] Sair.\n\n");

        printf ("Escolha sua opcao: ");
        scanf ("%d", &option);
        getchar ();

        system (CLEAR_COMMAND);
    }

    if (option == 3)
    {
        system (CLEAR_COMMAND);

        printf ("Saindo do Hortifruti, obrigado pela sua visita !");
        sleep (TIME_TO_EXIT);

        system ("exit");

        return 1;
    }

    if (option == 1)
    {
        system (CLEAR_COMMAND);

        char admin_name[25];
        char admin_pass[25];

        printf ("Informe o login: ");
        fgets (admin_name, 25, stdin);
        admin_name[strcspn (admin_name, "\n")] = '\0';

        printf ("Informe a senha: ");
        fgets (admin_pass, 25, stdin);
        admin_pass[strcspn (admin_pass, "\n")] = '\0';

        char *content = readFile ("src/users/admins.json");

        User user = findUser (content, admin_name);
        free (content);

        if (strlen (user.name) < 1)
        {
            printf ("\nUsuario nao encontrado.");

            return 1;
        }

        if (strcmp (user.pass, admin_pass) != 0)
        {
            printf ("\nSenha incorreta.");

            return 1;
        }

        printf ("\nLogado com sucesso, bem - vindo(a) de volta %s.", user.name);
    }
    else if (option == 2)
    {
        system (CLEAR_COMMAND);

        char user_name[25];
        char user_pass[25];

        printf ("Informe o login: ");
        fgets (user_name, 25, stdin);
        user_name[strcspn (user_name, "\n")] = '\0';

        printf ("Informe a senha: ");
        fgets (user_pass, 25, stdin);
        user_pass[strcspn (user_pass, "\n")] = '\0';

        char *content = readFile ("src/users/customers.json");

        User user = findUser (content, user_name);
        free (content);

        if (strlen (user.name) < 1)
        {
            printf ("\nUsuario nao encontrado.");

            return 1;
        }

        if (strcmp (user.pass, user_pass) != 0)
        {
            printf ("\nSenha incorreta.");

            return 1;
        }

        printf ("\nLogado com sucesso, bem - vindo(a) de volta %s.", user.name);
    }
    
    return 0;
}