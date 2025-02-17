#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../parser/tvsctld_parser.h"
#include "tvsctld_commands.h"
#include "../tvsctld.h"

int handle_inc_command(tvsctl_command *command, char *data_in_buffer, int out_fd);
int handle_dec_command(tvsctl_command *command, char *data_in_buffer, int out_fd);
int handle_reset_command(tvsctl_command *command, char *data_in_buffer, int out_fd);
int handle_stop_command(tvsctl_command *command, char *data_in_buffer, int out_fd);
int handle_start_command(tvsctl_command *command, char *data_in_buffer, int out_fd);
int handle_status_command(tvsctl_command *command, char *data_in_buffer, int out_fd);
int send_command(int out_fd, char *data_in_buffer, int len);

command_entry_t command_table[] = {
    {INC_CMD, handle_inc_command, parse_inc_command},
    {DEC_CMD, handle_dec_command, parse_dec_command},
    {RESET_CMD, handle_reset_command, parse_reset_command},
    {STOP_CMD, handle_stop_command, parse_stop_command},
    {START_CMD, handle_start_command, parse_start_command},
    {STATUS_CMD, handle_status_command, parse_status_command},
    {NULL, NULL}};

tvsctl_command handle_command(char *data_in_buffer, int out_fd)
{
    int argc = 0;

    char *argv[MAX_MSG_SIZE];

    char *token = strtok(data_in_buffer, " \n");

    while (token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, " \n");
    }

    if (argc < 1)
    {
        fprintf(stderr, "Received unknown command\n");
        exit(EXIT_FAILURE);
    }

    tvsctl_command command;
    command.command = NULL;
    command.base = DEFAULT_BASE;
    command.scale = DEFAULT_SCALE;
    command.delta = DEFAULT_DELTA_INC;
    command.db = DEFAULT_DB;
    command.err_msg = NULL;
    command.err_code = 0;

    for (int i = 0; command_table[i].command_str != NULL; i++)
    {
        if (strcmp(argv[0], command_table[i].command_str) == 0)
        {
            command_table[i].parser(argc, argv, &command);

            if (command.command == NULL)
            {
                command.err_code = COMMAND_PARSE_ERROR;
                return command;
            }

            if (command_table[i].handler(&command, data_in_buffer, out_fd) != 0)
            {
                command.err_code = SOCKET_ERROR;
                command.err_msg = "An error occurred while executing the command\n";
                return command;
            }

            return command;
        }
    }

    command.err_code = COMMAND_PARSE_ERROR;
    command.err_msg = "Unknown command\n";

    return command;
}

int handle_inc_command(tvsctl_command *command, char *data_in_buffer, int out_fd)
{
    int len = sprintf(data_in_buffer, "%s%s.sh %d\n", SCRIPT_PREFIX, command->command, command->delta);
    return send_command(out_fd, data_in_buffer, len);
}

int handle_dec_command(tvsctl_command *command, char *data_in_buffer, int out_fd)
{
    int len = sprintf(data_in_buffer, "%s%s.sh %d\n", SCRIPT_PREFIX, command->command, command->delta);
    return send_command(out_fd, data_in_buffer, len);
}

int handle_reset_command(tvsctl_command *command, char *data_in_buffer, int out_fd)
{
    int len = sprintf(data_in_buffer, "%s%s.sh %d %d\n", SCRIPT_PREFIX, command->command, command->scale, command->base);
    return send_command(out_fd, data_in_buffer, len);
}

int handle_stop_command(tvsctl_command *command, char *data_in_buffer, int out_fd)
{
    int len = 0;
    if (command->db)
    {
        len = sprintf(data_in_buffer, "%s%s.sh %s\n", SCRIPT_PREFIX, command->command, DB_FLAG);
    }
    else
    {
        len = sprintf(data_in_buffer, "%s%s.sh\n", SCRIPT_PREFIX, command->command);
    }
    return send_command(out_fd, data_in_buffer, len);
}

int handle_start_command(tvsctl_command *command, char *data_in_buffer, int out_fd)
{
    int len = sprintf(data_in_buffer, "%s%s.sh\n", SCRIPT_PREFIX, command->command);
    return send_command(out_fd, data_in_buffer, len);
}

int handle_status_command(tvsctl_command *command, char *data_in_buffer, int out_fd)
{
    int len = sprintf(data_in_buffer, "%s%s.sh\n", SCRIPT_PREFIX, command->command);
    return send_command(out_fd, data_in_buffer, len);
}

int send_command(int out_fd, char *data_in_buffer, int len)
{
    data_in_buffer[len] = '\0';

    FILE *fp = popen(data_in_buffer, "r");
    if (fp == NULL)
    {
        perror("Error executing command");
        return COMMAND_EXEC_ERROR;
    }

    int buffer_size = MAX_MSG_SIZE;

    char *data_out_buffer = (char *)malloc(buffer_size);

    if (data_out_buffer == NULL)
    {
        perror("Error allocating memory");
        pclose(fp);
        return COMMAND_EXEC_ERROR;
    }

    int total_read_len = 0;
    int read_len;

    while ((read_len = fread(data_out_buffer + total_read_len, 1, buffer_size - total_read_len - 1, fp)) > 0)
    {
        total_read_len += read_len;
        if (total_read_len >= buffer_size - 1)
        {
            buffer_size *= 2;
            char *new_buffer = (char *)realloc(data_out_buffer, buffer_size);
            if (new_buffer == NULL)
            {
                perror("Error reallocating memory");
                free(data_out_buffer);
                pclose(fp);
                return COMMAND_EXEC_ERROR;
            }
            data_out_buffer = new_buffer;
        }
    }

    data_out_buffer[total_read_len] = '\0';

    write(out_fd, data_out_buffer, total_read_len);

    free(data_out_buffer);
    pclose(fp);

    return 0;
}