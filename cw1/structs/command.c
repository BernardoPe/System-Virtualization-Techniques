#include "command.h"
#include "simple_command.h"
#include "parent_command.h"
#include <stdlib.h>
#include "../utils/alloc.h"

/**
 * Creates a new Command struct
 */
Command *new_command()
{
    Command *command = (Command *)safe_malloc(sizeof(Command));
    command->simple_commands = NULL;
    command->parent_command = NULL;
    command->num_commands = 0;
    return command;
}

void free_command(Command *command)
{
    if (command->simple_commands != NULL)
    {
        for (int i = 0; i < command->num_commands; i++)
        {
            free_simple_command(command->simple_commands[i]);
        }
        free(command->simple_commands);
    }
    if (command->parent_command != NULL)
    {
        free_parent_command(command->parent_command);
    }
    free(command);
}