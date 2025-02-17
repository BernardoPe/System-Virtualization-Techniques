#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int atoi_with_check(char *str, char *arg_name)
{
    int num = atoi(str);
    if (num == 0)
    {
        fprintf(stderr, "Invalid argument: %s\n", arg_name);
        return 0;
    }
    if (num < 0)
    {
        fprintf(stderr, "Argument %s must be positive\n", arg_name);
        return 0;
    }
    return num;
}
