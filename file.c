#include "config.h"

#include <stdio.h>
#include <stdlib.h>

char* readFile ()
{
    FILE *file = fopen (JSON_PATH, "r");

    fseek (file, 0, SEEK_END);
    long size = ftell (file);
    fseek (file, 0, SEEK_SET);

    char *content = malloc (size + 1);
    fread (content, 1, size, file);
    content[size] = '\0';

    fclose (file);

    return content;
}

int* writeFile (const char* content) {
    FILE *file = fopen (JSON_PATH, "w");

    fprintf (file, "%s", content);
    fclose (file);

    return 0;   
}