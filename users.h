#ifndef USERS_H
#define USERS_H

typedef struct {
    char username[25];
    char password[25];

    int administrator;
} Users;

Users auth;
Users findUser (const char *content, const char *search);

#endif