#ifndef PROCESS_H
#define PROCESS_H

void run_command(char *const argv[]);
char *run_command_with_input(char *const argv[], const char *input);
char *capture_command_output(char *const argv[]);

#endif
