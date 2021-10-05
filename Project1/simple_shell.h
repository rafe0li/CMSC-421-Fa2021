#ifndef SIMPLE_SHELL_H
#define	SIMPLE_SHELL_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	extern void shell();

	// Takes in input
	extern char** shell_Input();

	// Parses input
	extern char** parse_Input(char* buff, int size);

	extern void ls_Func(char** args);

	extern int exit_Func(int status, int with);

	extern void echo_Func(char** args, int op_e);
#ifdef __cplusplus
}
#endif

#endif