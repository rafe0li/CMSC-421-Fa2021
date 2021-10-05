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
char* LS_CMD = "ls";
char* EXIT_CMD = "exit";
char* ECHO_CMD = "echo";
char* PROC_CMD = "proc";
char* FILE_CMD = "/";
char* AST = "*";
/* How many args the user has entered
 * Incremented during input parsing
*/
int ARG_C = 0;
int BUF_SIZE = 32;


void shell() {
	printf("Welcome to simple_shell!\nThis C program mimics a shell like Bash.\nJust type in a command to the shell, after $. Have fun!\n\n");
	bool running = true;
	// Allocate buffer for user input

	// Loop that controls shell behavior
	// NO REASON TO CHANGE PERROR UNLESS ASKED TO
	while (running) {
		char* buffer;
		size_t buf_size = 32;
		int size = 0;
		buffer = (char*)malloc(buf_size * sizeof(char));
		buffer = shell_Input(buffer, buf_size);
		size = count_spaces(buffer);
		char** args = parse_Input(buffer, size);

		// Branches to different commands
		// strcmp returns 0 if the strings are equal
		if (!strcmp(args[0], LS_CMD)) {
			if (ARG_C == 1) {
				ls_Func(args, 1);
			}
			else if (ARG_C == 2) {
				if (args[1][0] == '-') {
					char* p = (char*)(args[1] + 1);
					// Checks if option is supported
					if (*p != 'a' && *p != 'A' && *p != 'r' && *p != 'R' && *p != 'S' && *p != 's' && *p != 'l' && *p != 'i' && *p != 'g' && *p != 'T') {
						fprintf(stderr, "\n ERROR\nOption \"%s\" is not supported\n\n", args[1]);
					}
					ls_Func(args, 0);
				}
				else {
					fprintf(stderr, "\n ERROR\nNo options available or none selected\n\n");
				}
			}
			else {
				fprintf(stderr, "\n ERROR\nToo many arguments\n\n");
			}
		}

		else if (!strcmp(args[0], EXIT_CMD)) {
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

		else if (!strcmp(args[0], ECHO_CMD)) {
			// echo
			if (ARG_C == 1)
				printf("\n");
			// echo "Hello \t"
			else if (ARG_C == 2) {
				// echo *
				if (!strcmp(args[1], AST)) {
					ls_Func(args, 0);
				}
				else {
					echo_Func(args[1], 0);
				}
			}
			// echo -e "Hello \t"
			else if (ARG_C == 3) {
				// Checks if -e selected
				if (args[1][0] == '-') {
					int op_e = 0;
					char* p = (char*)(args[1] + 1);

					// Checks which option it was
					while (*p) {
						if (*p == 'e') {
							op_e = 1;
						}
						else {
							perror("Option not available");
							exit(EXIT_FAILURE);
						}
						p++;
					}
					echo_Func(args[2], op_e);
				}
				else {
					perror("No options selected or options not available");
					exit(EXIT_FAILURE);
				}
			}
		}

		else {
			printf("\n simple_shell: %s : command not found\n\n", args[0]);
		}
		ARG_C = 0;
		printf("\n");
		free(buffer);
		free(args);
	}
}

char* shell_Input(char* buff, size_t buf_size) {
	printf("SSHELL$ ");
	// Index for char
	int i = 0;
	// Char
	int c;
	// End of buffer
	int end = buf_size * sizeof(char);
	// How many times the buffer has been reallocated
	int grow = 0;

	c = fgetc(stdin);
	while (c != '\n' && c != EOF) {
		buff[i] = c;
		i++;
		// If next char is not EOF, check if there's enough space
		c = fgetc(stdin);
		if (c != '\n' && c != EOF) {
			if (i == end) {
				grow++;
				buff = realloc(buff, buf_size * 2 * sizeof(char));
			}
		}
	} 
	if (grow != 0) {
		end = end * grow;
		BUF_SIZE = BUF_SIZE * grow;
	}
	// Initializes remainder of string to null values
	fill_Buff(buff, i);

	return buff;
}

/* Separates expressions from user input, stores results
 * into 2D char arrays (essentially array of strings).
 * WARNING: FUNCTION DOESN'T PARSE THE LAST NEWLINE CHAR CORRECTLY
 * 
 * @param[in] buff Input to parse into command/arguments
 * @return 2D char array of parsed results, empty array for no input
 * 
 * 
 * Citations
 * https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
*/
char** parse_Input(char* buff, int size) {
	// Allocate memory for a 2D char array, string array in C
	char** arr = create_Char_Array(size);
	// Takes in a char* once, and then each consecutive call returns
	// strings delimited by the specified char
	// sPtr is savepointer, special pointer used by strtok_r
	char* sPtr;
	char* nextWord = (char*) malloc(BUF_SIZE * sizeof(char));
	nextWord = strtok_r(buff, " ", &sPtr);
	fill_Buff(nextWord, strlen(nextWord));
	int row = 0;
	
	// Loop until no more tokens (spaces) found
	while (nextWord != NULL) {
		arr[row] = nextWord;
		row++;
		ARG_C++;
		nextWord = (char*)realloc(nextWord, BUF_SIZE * sizeof(char));
		nextWord = strtok_r(NULL, " ", &sPtr);
		fill_Buff(nextWord, strlen(nextWord));
	}
	// Arg lists have to be null terminated
	if (row == 0) {
		arr[row + 1] = (void*)0;
	}
	else {
		arr[row] = (void*)0;
	}
	
	free(nextWord);
	BUF_SIZE = 32;
	return arr;
}

char* fill_Buff(char* buff, int index) {
	while (index < (int) sizeof(buff)) {
		buff[index] = NULL;
		index++;
	}
	return buff;
}

char** create_Char_Array(int m) {
	char **arr = (char**)calloc (m, sizeof(char *));
	for (int i = 0; i < m; i++) {
		arr[i] = (char*)calloc(m, sizeof(char*));
	}
	return arr;
}

void ls_Func(char** args, int option) {
	int status;
	if (option) {
		if (fork() == 0)
			execvp(LS_CMD, args);
		else
		{
			wait(&status);
		}
	}
	else {
		if (fork() == 0)
			execvp(LS_CMD, args);
		else
		{
			wait(&status);
		}
	}
}

int exit_Func(int status, int with) {
	if (with) {
		exit(status);
	}
	exit(EXIT_SUCCESS);
}

void echo_Func(char* buff, int op_e) {
	if (op_e) {
		buff = unescape(buff, stderr);
		printf("%s", buff);
	}
	else {
		printf("%s", buff);
	}
}

/* argv/argc are how command line args are passed to main 
 * in C. Error case is for requirement 1, shell program 
 * should take no arguments
 */
int main(int argc, char *argv[]) {
	if (argc > 1) {
		fprintf(stderr, "ERROR\n simple_shell cannot be called with command line arguments, %d found \n\n", argc - 1);
		exit(EXIT_FAILURE);
	}

	shell();

	exit(EXIT_SUCCESS);
}