#ifndef TVSCTLD_PARSER_H
#define TVSCTLD_PARSER_H

#include <stdbool.h>

#define DELTA_ARG "delta"
#define SCALE_ARG "scale"
#define BASE_ARG "base"

#define INC_CMD "inc"
#define DEC_CMD "dec"
#define RESET_CMD "reset"
#define STOP_CMD "stop"
#define START_CMD "start"
#define STATUS_CMD "status"

#define RESET_USAGE "Usage: reset <scale> <base>\n"
#define INC_USAGE "Usage: inc <delta>\n"
#define DEC_USAGE "Usage: dec <delta>\n"
#define STOP_USAGE "Usage: stop [-db]\n"
#define START_USAGE "Usage: start\n"
#define STATUS_USAGE "Usage: status\n"

#define DB_FLAG "-db"

typedef struct tvsctl_command
{
    char *command;
    int scale;
    int base;
    int delta;
    bool db;
    char *err_msg;
    int err_code;
} tvsctl_command;

typedef void (*command_parser_t)(int argc, char *argv[], tvsctl_command *command);

void parse_inc_command(int argc, char *argv[], tvsctl_command *command);
void parse_dec_command(int argc, char *argv[], tvsctl_command *command);
void parse_reset_command(int argc, char *argv[], tvsctl_command *command);
void parse_stop_command(int argc, char *argv[], tvsctl_command *command);
void parse_start_command(int argc, char *argv[], tvsctl_command *command);
void parse_status_command(int argc, char *argv[], tvsctl_command *command);

#endif // TVSCTLD_PARSER_H
