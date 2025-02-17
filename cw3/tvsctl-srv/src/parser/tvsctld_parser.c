#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tvsctld_parser.h"
#include "../handler/tvsctld_commands.h"
#include "../utils/utils.h"

#define MAX_LINE_SIZE 1024

void parse_inc_command(int argc, char *argv[], tvsctl_command *command)
{
    switch (argc)
    {
    case 1:
        command->command = INC_CMD;
        command->delta = DEFAULT_DELTA_INC;
        break;
    case 2:
        command->delta = atoi_with_check(argv[1], DELTA_ARG);
        if (command->delta)
        {
            command->command = INC_CMD;
        }
        break;
    default:
        command->err_msg = INC_USAGE;
        command->err_code = COMMAND_PARSE_ERROR;
        break;
    }
}

void parse_dec_command(int argc, char *argv[], tvsctl_command *command)
{
    switch (argc)
    {
    case 1:
        command->command = DEC_CMD;
        command->delta = DEFAULT_DELTA_DEC;
        break;
    case 2:
        command->delta = atoi_with_check(argv[1], DELTA_ARG);
        if (command->delta)
        {
            command->command = DEC_CMD;
        }
        break;
    default:
        command->err_msg = DEC_USAGE;
        command->err_code = COMMAND_PARSE_ERROR;
        break;
    }
}

void parse_reset_command(int argc, char *argv[], tvsctl_command *command)
{
    switch (argc)
    {
    case 1:
        command->command = RESET_CMD;
        command->scale = DEFAULT_SCALE;
        command->base = DEFAULT_BASE;
        break;
    case 2:
        command->scale = atoi_with_check(argv[1], SCALE_ARG);
        command->base = DEFAULT_BASE;
        if (command->scale)
        {
            command->command = RESET_CMD;
            command->base = DEFAULT_BASE;
        }
        break;
    case 3:
        command->scale = atoi_with_check(argv[1], SCALE_ARG);
        command->base = atoi_with_check(argv[2], BASE_ARG);
        if (command->scale && command->base)
        {
            command->command = RESET_CMD;
        }
        break;
    default:
        command->err_msg = RESET_USAGE;
        command->err_code = COMMAND_PARSE_ERROR;
        break;
    }
}

void parse_stop_command(int argc, char *argv[], tvsctl_command *command)
{
    printf("parsing stop command\n");
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    switch (argc)
    {
    case 1:
        command->command = STOP_CMD;
        command->db = DEFAULT_DB;
        break;
    case 2:
        if (strcmp(argv[1], DB_FLAG) == 0)
        {
            command->db = true;
            command->command = STOP_CMD;
        }
        else
        {
            command->err_code = COMMAND_PARSE_ERROR;
            command->err_msg = STOP_USAGE;
        }
        break;
    default:
        command->err_msg = STOP_USAGE;
        command->err_code = COMMAND_PARSE_ERROR;
        break;
    }
}

void parse_start_command(int argc, char *argv[], tvsctl_command *command)
{
    switch (argc)
    {
    case 1:
        command->command = START_CMD;
        break;
    default:
        command->err_msg = START_USAGE;
        command->err_code = COMMAND_PARSE_ERROR;
        break;
    }
}

void parse_status_command(int argc, char *argv[], tvsctl_command *command)
{
    switch (argc)
    {
    case 1:
        command->command = STATUS_CMD;
        break;
    default:
        command->err_msg = STATUS_USAGE;
        command->err_code = COMMAND_PARSE_ERROR;
        break;
    }
}
