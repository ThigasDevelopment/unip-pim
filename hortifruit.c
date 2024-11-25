#include "file.h"
#include "cJSON.h"
#include "config.h"
#include "hortifruit.h"

#include <stdio.h>

#include <stdlib.h>
#include <unistd.h>

int setBalance (int balance) {
    const int actual = getBalance ();

    if (actual == balance) {
        return 1;
    }

    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);
    free (content);

    cJSON *system = cJSON_GetObjectItem (hortifruit, "system");
    cJSON *item_balance = cJSON_GetObjectItem (system, "balance");

    cJSON_SetNumberValue (item_balance, balance);

    char *new_content = cJSON_Print (hortifruit);
    writeFile (new_content);
    free (new_content);

    cJSON_Delete (hortifruit);

    return 0;
}

int getBalance () {
    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);
    free (content);

    cJSON *system = cJSON_GetObjectItem (hortifruit, "system");
    cJSON *balance = cJSON_GetObjectItem (system, "balance");

    cJSON_Delete (hortifruit);

    return (int)balance -> valuedouble;
}