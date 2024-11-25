#ifndef CONFIG_H
#define CONFIG_H

#define HORTIFRUIT_NAME "PIM"

#define JSON_PATH "src/jsons/hortifruit.json"

#define TIME_TO_EXIT 3
#define TIME_TO_ACTION 1
#define TIME_TO_CHANGE 2

#ifdef __linux__
    #define CLEAR_COMMAND "clear"
#elif _WIN32
    #define CLEAR_COMMAND "cls"
#endif

#endif