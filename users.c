#include "cJSON.h"
#include "users.h"

#include <stdio.h>
#include <string.h>

Users auth = { "Thiago", "1234", 1 };

Users findUser (const char *content, const char *search)
{
    Users new_user = { "", "", 0 };

    cJSON *json = cJSON_Parse (content);

    if (json == NULL) {
        return new_user;
    }

    cJSON *users = cJSON_GetObjectItem (json, "users");

    int size = cJSON_GetArraySize (users);

    for (int i = 0; i < size; i++) {
        cJSON *item = cJSON_GetArrayItem (users, i);

        cJSON *name = cJSON_GetObjectItem (item, "name");
        cJSON *pass = cJSON_GetObjectItem (item, "pass");
        cJSON *type = cJSON_GetObjectItem (item, "type");

        if (strcmp (name -> valuestring, search) == 0) {
            strcpy (new_user.username, name -> valuestring);
            strcpy (new_user.password, pass -> valuestring);

            new_user.administrator = type -> valueint;

            break;
        }
    }

    cJSON_Delete (users);

    return new_user;
}