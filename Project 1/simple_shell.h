/*@simple_shell.h
* @author Rafael Li, AA31613, rafaell1@umbc.edu
* CMSC 421, Section 01
* Project 1 C Shell
*
* C program that parses command line args. Functions control the
* shell itself, input parsing, reading proc files, and exiting with 
* a return code.
*/
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
	extern void call_Bash(char** args, int absolute);

	// Displays info from proc files
	extern void proc_Func(char** args);

	// Exits out of shell
	extern int exit_Func(int status, int with);
#ifdef __cplusplus
}
#endif
