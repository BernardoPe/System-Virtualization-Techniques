#ifndef PARSE_H
#define PARSE_H

#include "../structs/command.h"
#include "../structs/simple_command.h"
#include "../structs/parent_command.h"

#define OUTPUT_REDIRECTION_TOKEN '>'
#define INPUT_REDIRECTION_TOKEN '<'
#define PIPE_TOKEN "|"
#define DELIMITERS " \t\r\n\a"

// Parses a command line into a Command struct
Command *parse_command(char *line);

// Parses a parent command line into a Command struct
Command *parse_parent_command(char *line, int command_code);

// Parses a simple commands line into a Command struct
Command *parse_simple_commands(char *line);

// Parses a simple command line into a SimpleCommand struct
SimpleCommand *parse_simple_command(char *line);

// Gets the parent command code from a line
int get_parent_command(char *line);

// Checks if a string starts with a given match
int string_start_match(char *str, char *match);

// Removes leading and trailing whitespaces from a string
void trim(char *str);

// Checks if a string is empty or contains only whitespaces
int is_blank(char *str);

// Splits a string into tokens using a delimiter
int split(char *str, char *delim, char ***tokens, int tokens_size);

#endif // PARSE_H