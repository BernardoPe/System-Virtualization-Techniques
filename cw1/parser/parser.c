#include "./parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../utils/alloc.h"

Command *parse_command(char *line)
{
    int command_code = get_parent_command(line);
    if (command_code != -1)
    {
        return parse_parent_command(line, command_code);
    }
    else
    {
        return parse_simple_commands(line);
    }
}

Command *parse_simple_commands(char *line)
{

    int commands_size = STARTING_COMMANDS_SIZE;

    char **commands = (char **)safe_malloc(commands_size * sizeof(char *));

    int command_count = split(line, PIPE_TOKEN, &commands, commands_size);

    if (command_count == 0)
    {
        free(commands);
        return NULL;
    }

    Command *command = new_command();
    command->simple_commands = (SimpleCommand **)safe_malloc(command_count * sizeof(SimpleCommand *));

    for (int i = 0; i < command_count; i++)
    {
        if (!is_blank(commands[i]))
        {
            command->simple_commands[command->num_commands] = parse_simple_command(commands[i]);
            command->num_commands = command->num_commands + 1;
        }
    }

    free(commands);

    return command;
}

SimpleCommand *parse_simple_command(char *line)
{
    SimpleCommand *command = new_simple_command();
    int args_size = STARTING_ARGS_SIZE;

    char *token = strtok(line, DELIMITERS);

    while (token != NULL)
    {
        trim(token);

        if (is_blank(token))
        {
            token = strtok(NULL, DELIMITERS);
            continue;
        }

        switch (token[0])
        {
        case OUTPUT_REDIRECTION_TOKEN:
            command->output = strtok(NULL, DELIMITERS);
            break;
        case INPUT_REDIRECTION_TOKEN:
            command->input = strtok(NULL, DELIMITERS);
            break;
        default:
            command->args[command->argc++] = token;
            break;
        }

        if (command->argc >= args_size)
        {
            args_size *= 2;
            command->args = (char **)safe_realloc(command->args, args_size * sizeof(char *));
        }

        token = strtok(NULL, DELIMITERS);
    }

    command->args[command->argc] = NULL; // Null-terminate the args array for execvp

    return command;
}

Command *parse_parent_command(char *line, int command_code)
{
    ParentCommand *parent_command = new_parent_command();
    parent_command->command_code = command_code;

    char *token = strtok(line, DELIMITERS);
    int args_size = STARTING_ARGS_SIZE;

    while (token != NULL)
    {
        if (parent_command->argc >= args_size)
        {
            args_size *= 2;
            parent_command->args = (char **)safe_realloc(parent_command->args, args_size * sizeof(char *));
        }
        parent_command->args[parent_command->argc++] = token;
        token = strtok(NULL, DELIMITERS);
    }

    Command *command = new_command();
    command->parent_command = parent_command;
    command->num_commands = 1;

    return command;
}

int get_parent_command(char *line)
{
    if (string_start_match(line, CD_COMMAND))
    {
        return CD_COMMAND_CODE;
    }
    else if (string_start_match(line, EXIT_COMMAND))
    {
        return EXIT_COMMAND_CODE;
    }
    return -1;
}

int string_start_match(char *str, char *match)
{
    return strncmp(str, match, strlen(match)) == 0;
}

void trim(char *str)
{
    int begin = 0, end = strlen(str) - 1;

    while (isspace((unsigned char)str[begin]))
        begin++;

    while (end >= begin && isspace((unsigned char)str[end]))
        end--;

    int i;
    for (i = begin; i <= end; i++)
    {
        str[i - begin] = str[i];
    }
    str[i - begin] = '\0';
}

int split(char *str, char *delim, char ***tokens, int tokens_size)
{
    int count = 0;

    char *token = strtok(str, delim);
    while (token != NULL)
    {
        if (count >= tokens_size)
        {
            tokens_size *= 2;
            *tokens = (char **)safe_realloc(*tokens, tokens_size * sizeof(char *));
        }
        (*tokens)[count++] = token;
        token = strtok(NULL, delim);
    }

    return count;
}

int is_blank(char *str)
{
    while (*str != '\0')
    {
        if (!isspace((unsigned char)*str))
            return 0;
        str++;
    }
    return 1;
}