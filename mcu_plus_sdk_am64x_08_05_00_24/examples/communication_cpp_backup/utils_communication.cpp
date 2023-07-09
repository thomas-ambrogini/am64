#include "utils_communication.hpp"



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


uint16_t str_to_int16_t (char * str){
    int myInt(std::stoi(str));
    return static_cast<uint16_t>(myInt);
}


uint32_t str_to_coreId  (char * str){
    if (strcmp(str,"R5_0_0") == 0) {
        return CSL_CORE_ID_R5FSS0_0;
    }
    else if (strcmp(str, "R5_0_1") == 0) {
        return CSL_CORE_ID_R5FSS0_1;
    }
    else if (strcmp(str, "R5_1_0") == 0) {
        return CSL_CORE_ID_R5FSS1_0;
    }
    else if (strcmp(str, "R5_1_1") == 0) {
        return CSL_CORE_ID_R5FSS1_1;
    }
    else if (strcmp(str, "M4") == 0) {
        return CSL_CORE_ID_M4FSS0_0;
    }
    else if (strcmp(str, "A53_0") == 0) {
        return CSL_CORE_ID_A53SS0_0;
    }
    else {
        return -1;
    }
}

