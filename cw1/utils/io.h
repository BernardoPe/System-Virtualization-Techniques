#ifndef IO_H
#define IO_H
#include <unistd.h>

int safe_open(const char *pathname, int flags, mode_t mode);

void safe_close(int fd);

#endif // FILE_H