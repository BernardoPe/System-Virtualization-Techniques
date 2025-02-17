#include <stdio.h>

#define KB (1024)

char data[512 * KB] = {1};

void affect_data(int nbytes, int page_size) {
    for (int i = 0; i < nbytes; i += page_size) {
        data[i] = 0;
    }
}