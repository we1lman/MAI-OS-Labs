#include "string_utils.h"
#include <string.h>

void invert_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    
    size_t end = (len > 0 && str[len - 1] == '\n') ? len - 2 : len - 1;
    size_t start = 0;
    char temp;
    while (start < end) {
        temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}
