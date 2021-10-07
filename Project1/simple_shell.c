/*@simple_shell.c
* @author Rafael Li, rafaell1@umbc.edu
* 
* C program that mimics the behavior of a shell.
* Takes no arguments on start, and will take a variety
* of args depending on the commands that are invoked by the user.
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "simple_shell.h"
#include "utils.h"

// Globals for matching the user's commands
char* EXIT_CMD = "exit";
char* PROC_CMD = "proc";
char* PROC_FILES = "/proc/";
// Number of args entered, used by parsing algorithm
int ARG_C = 0;


void shell() {
	printf("Welcome to simple_shell!\nThis C program mimics a shell like Bash.\nJust type in a command to the shell, after $. Have fun!\n\n");
	bool running = true;

	// Loop that controls shell behavior
	// NO REASON TO CHANGE PERROR UNLESS ASKED TO
	while (running)   {
		// Create arg array from user
		char** args = shell_Input();

		// If not exit or proc, attempt to run command
		// strcmp returns 0 if the strings are equal

		if (!strcmp(args[0], EXIT_CMD)) {
			if (ARG_C == 1) {
				free(*args);
				free(args);
				exit_Func(0, 0);
			}
			else if (ARG_C == 2) {
				// Stores the argument the user wants to exit with
				int stat = atoi(args[1]);
				// atoi returns 0 if the val cannot be parsed
				if (stat != 0) {
					// Check if value is valid exit code
					if (stat < 0 || stat > 255) {
						free(*args);
						free(args);
						exit_Func(stat, 1);
					}
					else {
						fprintf(stderr, "\n ERROR\nExit code \"%s\" is invalid. Must be 0-255\n\n", args[1]);
					}
				}
				// CASE "exit 0"
				else if (!strcmp(args[1], "0")) {
					free(*args);
					free(args);
					exit_Func(0, 0);
				}
				else {
					fprintf(stderr, "\n ERROR\nExit code \"%s\" is invalid/cannot be parsed\n\n", args[1]);
				}
			}
			else {
				fprintf(stderr, "\nERROR\nInvalid arguments\n\n");
				exit(EXIT_FAILURE);
			}
		}

		else if (!strcmp(args[0], PROC_CMD)) {
			if (ARG_C == 2) {
				proc_Func(args);
			}
			else {
				fprintf(stderr, "\nERROR\nInvalid arguments\n\n");
				exit(EXIT_FAILURE);
			}
		}

		else {
			call_Bash(args);
		}
		destroy_Arr(args, ARG_C);
		ARG_C = 0;
		printf("\n");
	}
}

char** shell_Input() {
	char* buffer = (char*)calloc(1, sizeof(char*));
	// Index for char
	int i = 0;
	int c;
	// Whether in quoted text or not
	int in_Quotes = 0;
	

	printf("SSHELL$ ");
	c = fgetc(stdin);
	// Initializes arg counter
	ARG_C++;
	while (c != '\n' && c != EOF) {
		// Counts the number of unquoted spaces, or args
		if (c == ' ') {
			if (!in_Quotes) {
				ARG_C++;
			}
		}
		if (c == '"') {
			if (in_Quotes) {
				in_Quotes = 0;
			}
			else {
				in_Quotes = 1;
			}
		}

		buffer[i] = c;
		i++;
		c = fgetc(stdin);
		// Allocates a new byte per char entered
		if (c != '\n' && c != EOF) {
			buffer = (char*)realloc(buffer, sizeof(char*) * i);
		}
	}
	
	//printf("\nSHELL_INPUT() BUFFER SIZE: [%d]\n\n", end);
	//printf("\nSHELL_INPUT() BUFFER: [%s]\n\n", buffer);
	char** args = parse_Input(buffer, ARG_C);

	if (buffer) {
		free(buffer);
	}
	
	return args;
}

/* Separates expressions from user input, stores results
 * into 2D char arrays.
 * 
 * @param[in] buff Input to parse into command/arguments
 * @return 2D char array of parsed results, empty array for no input
*/
char** parse_Input(char* buff, int size) {
	// Allocate memory for a 2D char array (string array in C)
	// size + 1 to add NULL at end, exec only accepts null-terminated arg lists
	char** arr = (char**)calloc(size + 1, sizeof(char*));
	// Char to be entered into token
	char c;
	// Token from buffer
	char* token = (char*)calloc(1, sizeof(char*));
	// Array index
	int arr_I = 0;
	// Buffer index
	int buff_I = 0;
	// Token index
	int tok_I = 0;
	// Size of token, allocates only as much as needed
	int tok_size = 2;
	while (buff_I <= (int)strlen(buff)) {
		c = buff[buff_I];
		if (c != ' ') {
			token = (char*)realloc(token, sizeof(char*) * tok_size);
			token[tok_I] = c;
			buff_I++;
			tok_I++;
			tok_size++;
		}
		if (c == ' ' || buff_I == (int)strlen(buff)) {
			arr[arr_I] = (char*)calloc(tok_size, sizeof(char*));
			strcpy(arr[arr_I], token);
			arr_I++;
			buff_I++;
			tok_I = 0;
			tok_size = 1;
		}
	}
	
	// Arg arrays must be null terminated for exec
	arr[arr_I] = (char*)calloc(1, sizeof(char*));
	arr[arr_I] = NULL;

	if (token) {
		free(token);
	}
	
	return arr;
}

void destroy_Arr(char** arr, int size) {
	for (int i = 0; i < size; i++) {
		free(arr[i]);
	}
	free(arr);
}

/* Uses execvp to call system with argument array.
 *
 * @param[in] args Parsed argument array from user
*/
void call_Bash(char** args) {
	int status;
	if (fork() == 0) {
		execvp(args[0], args);
	}
	else {
		wait(&status);
	}
}

/* Prints data from Linux's proc system.
 *
 * @param[in] args Parsed arg array specifying which proc file
*/
void proc_Func(char** args) {
	FILE* fp;
	char* line = NULL;
	char* file_Name = (char*) malloc(6 * sizeof(args[1]) * sizeof(char*));
	strcpy(file_Name, PROC_FILES);
	strcat(file_Name, args[1]);
	size_t len = 0;
	ssize_t read;
	fp = fopen(file_Name, "r");
	if (fp == NULL)
		exit(EXIT_FAILURE);

	while ((read = getline(&line, &len, fp)) != -1) {
		fprintf(stdout, "%s", line);
	}

	if (line) {
		free(line);
	}
	if (file_Name) {
		free(file_Name);
	}

	fclose(fp);
}

/* Exits either with user specified return code
 * or normal return code.
 *
 * @param[in] status User specified exit code
 * @param[in] with Bool whether exiting normally or with user code
*/
int exit_Func(int status, int with) {
	if (with) {
		exit(status);
	}
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		fprintf(stderr, "ERROR\n simple_shell cannot be called with command line arguments, %d found \n\n", argc - 1);
		exit(EXIT_FAILURE);
	}

	shell();

	exit(EXIT_SUCCESS);
}