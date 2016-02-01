#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#include "gosh.h"

extern int errno;
char input[1024];
int main() {

  struct command_t command_a;

  /* Problem 3
     retval = start_sig_catchers();
  */

  while(1) {

    printf("gosh> ");

    /* this getchar() function is just here so that the shell 'works'
       without any modification.  comment this function call out
       before you start working on either of the two funtions below
       for input processing */
    //getchar();
		int retval = init_command(&command_a);

    /* Problem 1
       retval = simple_accept_input(&command_a);
    */
		retval = simple_accept_input(&command_a);

    print_command(&command_a, "c1");

    /*
       If the input processing was sucessful, call a fork/exec
       function.  Otherwise, exit the program
    */
    if (retval == 0) {
	 		simple_fork_command(&command_a);
    } else if (retval == 1) {
      exit(0);
    }

  }
  exit(0);
}

/* Helper Function.  Initialized a command_t struct */
int init_command(struct command_t *cmd) {
  cmd->num_args = 0;
  cmd->args[0] = NULL;
  cmd->outfile[0] = '\0';
  cmd->infile[0] = '\0';
  return(0);
}

/* Helper Function.  Print out relevent info stored in a command_t struct */
int print_command(struct command_t *cmd, const char *tag) {
  int i;

  for (i=0; i<cmd->num_args; i++) {
    printf("%s %d: %s\n", tag, i, cmd->args[i]);
  }

  if (cmd->outfile[0] != '\0') {
    printf("%s outfile: %s\n", tag, cmd->outfile);
  }

  if (cmd->infile[0] != '\0') {
    printf("%s infile: %s\n", tag, cmd->infile);
  }

  return(0);
}


/* Problem 1: read input from stdin and split it up by " " characters.
   Store the pieces in the passed in cmd_a->args[] array.  If the user
   inputs 'exit', return a 1.  If the user inputs nothing (\n), return
   a value > 1.  If the user inputs somthing else, return a 0. */
int simple_accept_input(struct command_t *cmd_a) {

	fgets (input, 1024, stdin);
	//printf("%s\n", input);
	char *temp;
  temp = strtok (input, " \n");
	//
	//printf("temp-->%s\n", temp);
	if (temp==NULL) {
		return (2);
	}
  while (temp != NULL)  {
		//printf("%d\n", 2);

		cmd_a->args[cmd_a->num_args] = temp;
		cmd_a->num_args+=1;
    temp = strtok (NULL, " \n");
		//printf("%d\n", 3);
  }
	//printf("%d\n", 4);
	char *arg0 = cmd_a->args[0];
	//printf("%d\n", 5);
	char * ex = {"exit\0"};
	//printf("%s%s\n", arg0, ex);

	if (strcmp(arg0, ex)==0) {
		//printf("%d\n", 6);
		return (1);
	}
	else {
		//printf("%d\n", 7);
		return(0); }
}

/* Problem 2: write a simple fork/exec/wait procedure that executes
   the command described in the passed in 'cmd' pointer. */
int simple_fork_command(struct command_t *cmd) {

		pid_t pid;
		int stat_val, child_pid;

    pid = fork();
    switch(pid)
    {
    case -1:
			printf("There was an error creating the child process. Exiting...");
      exit(1);
    case 0:
			execvp (cmd->args[0], cmd->args);
      break;
    default:

			child_pid = wait(&stat_val);
			if (child_pid<0) {
				printf("There was an error executing the command. Please try again\n");
			}
      break;
    }
	return(0);
}

/* Problem 3: set up all of your signal actions here */
int start_sig_catchers(void) {
  return(0);
}
