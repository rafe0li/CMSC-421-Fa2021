#ifndef SIMPLE_SHELL_H
#define	SIMPLE_SHELL_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	extern void shell();
	
	extern char** create_Char_Array(int m);

	extern char** parse_Input(char* buff, int size);

	extern void ls_Func(const char* dir, int op_a, int op_l);

	int exit_Func(int status, int with);
#ifdef __cplusplus
}
#endif

#endif