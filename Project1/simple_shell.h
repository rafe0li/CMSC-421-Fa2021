#ifndef SIMPLE_SHELL_H
#define	SIMPLE_SHELL_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	// Runs shell
	extern void shell();

	// Takes in input
	extern char** shell_Input();

	// Parses input
	extern char** parse_Input(char* buff, int size);

	// Frees 2D Char array
	extern void destroy_Arr(char** arr, int size);

	// Calls Bash with exec
	extern void call_Bash(char** args);

	// Displays info from proc files
	extern void proc_Func(char** args);

	// Exits out of shell
	extern int exit_Func(int status, int with);
#ifdef __cplusplus
}
#endif

#endif