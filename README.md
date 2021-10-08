# @author Rafael Li, AA31613, rafaell1@umbc.edu
# CMSC 421 Section 01
# simple_shell.c, simple_shell.h, utils.c, utils.h, Makefile, README.md

A C program that simulates the behavior of Bash by parsing input and calling exec.

Two functions deal with input: shell_Input() takes in an input line from the user,
and parse_Input() parses that input into an argument array.

Once the argument array is created, the program determines if either exit was called
or proc was called. 

If exit is called, the C program exits. One argument is allowed, a user specified return code.
If the return code is a. successfully parsed into an integer and b. is a valid return code (0-255 for UNIX systems).

If proc is called, the program attempts to open and read the specified proc binary.
If the specified file cannot be read or does not exist, an error is printed, otherwise
the file's data is printed to the console.

If neither exit nor proc is called, call_Bash() attempts to execute a system call by forking
a new process, detecting whether an absolute path to a binary is used to call the command ("/bin/ls")
or whether the command itself is just called. execv() is called for an absolute path, otherwise execvp() is called.
