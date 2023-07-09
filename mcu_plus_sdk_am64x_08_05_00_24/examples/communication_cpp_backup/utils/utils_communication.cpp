#include "utils.hpp"



char ** str_split(char * str, const char delimiter) {
    char ** result      = 0;
    size_t count        = 0;
    char* tmp           = str;
    char * last_comma   = 0;
    char delim[2];
    delim[0] = delimiter;
    delim[1] = 0;

    while (*tmp) {
        if (delimiter == *tmp) {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    count+= last_comma < (str + strlen(str) - 1);

    count++;

    result = (char **) malloc(sizeof(char*) * count);

    if (result) {
        size_t idx = 0;
        char * token = strtok(str, delim);

        while (token) {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}