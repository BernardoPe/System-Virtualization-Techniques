#ifndef TVSCTLD_COMMANDS_H
#define TVSCTLD_COMMANDS_H

#include <stdbool.h>
#include "../parser/tvsctld_parser.h"

#define DEFAULT_SCALE 1
#define DEFAULT_BASE 35000
#define DEFAULT_DELTA_INC 1
#define DEFAULT_DELTA_DEC 1
#define DEFAULT_DB false

#define COMMAND_PARSE_ERROR 1
#define SOCKET_ERROR 2
#define COMMAND_EXEC_ERROR 3

tvsctl_command handle_command(char *data_in_buffer, int out_fd);

typedef int (*command_handler_t)(tvsctl_command *command, char *data_in_buffer, int out_fd);

typedef struct
{
    const char *command_str;
    command_handler_t handler;
    command_parser_t parser;
} command_entry_t;

#endif // TVSCTLD_COMMANDS_H
