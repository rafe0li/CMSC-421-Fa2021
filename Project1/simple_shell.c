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
#include <dirent.h>
#include <errno.h>
#include "utils.h"

// Globals for matching the user's commands
char* LS_CMD = "ls";
char* EXIT_CMD = "exit";
char* ECHO_CMD = "echo";
char* PROC_CMD = "proc";
int ARG_C = 0;

char** create_Char_Array(int);
char** parse_Input(char*, int);

void shellLoop() {
	printf("Welcome to simple_shell!\n This C program mimics a shell like Bash.\n Just type in a command to the shell, after $. Have fun!\n\n");
	bool running = true;
	// Allocate buffer for user input
	char* buffer;
	size_t buf_size = 32;
	buffer = (char*)malloc(buf_size * sizeof(char));

	// Loop that controls shell behavior
	while (running) {
		printf("SSHELL$ ");
		// Reads in input and parses
		getline(&buffer, &buf_size, stdin);
		int size = count_spaces(buffer);
		char** args = parse_Input(buffer, size);

		// Branches to different commands
		// strcmp returns 0 if the strings are equal
		if (!strcmp(args[0], LS_CMD)) {
			ls_Func(".", 0, 0);
		}

		else if (strcmp(buffer, EXIT_CMD)) {
			exit_Func();
		}
		else {
			printf("\n simple_shell: %s : command not found", args[0]);
		}
		printf("\n");
	}
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
	char* nextWord = strtok(buff, " ");
	int row = 0;
	
	// Loop until no more tokens (spaces) found
	while (nextWord != NULL) {
		arr[row] = nextWord;
		row++;

		// Removes newline from last expression
		if (row == size) {
			int len = strlen(nextWord);
			if (nextWord[len - 1] == '\n') {
				nextWord[len - 1] = 0;
			}
		}

		nextWord = strtok(NULL, " ");
	}

	return arr;
}

char** create_Char_Array(int m) {
	char **arr = malloc (m * sizeof(char *));
	for (int i = 0; i < m; i++) {
		arr[i] = (char*)malloc(50);
	}
	return arr;
}

void destroyArray(float** arr) {
	free(*arr);
	free(arr);
}

void ls_Func(const char *dir, int op_a, int op_l) {
	struct dirent* d;
	DIR* dh = opendir(dir);
	if (!dh)
	{
		if (errno = ENOENT) {
			// If directory not found
			fprintf(stderr, "Directory doesn't exist");
		}
		else {
			// If directory is not readable
			fprintf(stderr, "Directory cannot be accessed");
		}
			
		exit(EXIT_FAILURE);
	}
	
	while ((d = readdir(dh)) != NULL) {
		// Continue if hidden files found
		if (!op_a && d->d_name[0] == '.')
			continue;
		printf("%s  ", d->d_name);
		if (op_l)
			printf("\n");
	}
	if (!op_l)
		printf("\n");
}

int exit_Func(int argc, char* argv[]) {
	return EXIT_SUCCESS;
}

/* argv/argc are how command line args are passed to main 
 * in C. Error case is for requirement 1, shell program 
 * should take no arguments
 */
int main(int argc, char *argv[]) {
	if (argc > 1) {
		fprintf(stderr, "Error: simple_shell cannot be called with any arguments, %d found", argc - 1);
		exit(EXIT_FAILURE);
	}

	shellLoop();

	free(argv);
	free(LS_CMD);
	free(EXIT_CMD);
	free(ECHO_CMD);
	free(PROC_CMD);

	return EXIT_SUCCESS;
}