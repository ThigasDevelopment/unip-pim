#ifndef CONFIG_H
#define CONFIG_H

#define MAX_BUFFER_SIZE 1024
#define HORTIFRUIT_NAME "PIM GROUP (7)"

#define TIME_TO_EXIT 3 // Time in seconds.
#define TIME_TO_CHANGE 2 // Time in seconds.

#ifdef __linux__
    #define CLEAR_COMMAND "clear"
#elif _WIN32
    #define CLEAR_COMMAND "cls"
#endif

#endif