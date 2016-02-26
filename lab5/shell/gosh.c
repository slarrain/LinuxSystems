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

	// ENV VARIABLE CASE
	char *var_s = strchr (input, '=');
	if (var_s!=NULL){
		/*
		if (putenv(input)!=0){
			printf("Error setting ENV variable");
		}
		*/
		char *temp1;
		temp1 = strtok(input, "=");
		//char *echo = "echo $";
		//sprintf(input, "%s%s", echo, temp1);

		//printf("%s --> %s\n",temp1, input);
		setenv(temp1, var_s+1, 1);
		//printf("ENV var SLE temp %s, var_s+1 %s, getenv %s\n", temp1, var_s+1, getenv(temp1));
		//input = "echo ";
		//strcat(input, "define new variable: success");
		cmd_a->args[0]="echo";
		cmd_a->args[1]="define new variable: success";
		cmd_a->num_args=2;
	}



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

	//'cd' case
	if (strcmp(cmd_a->args[0], "cd")==0) {
		chdir(cmd_a->args[1]);
				}

	//Sets next element of args to NULL
	cmd_a->args[cmd_a->num_args] = NULL;

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
		int stat_val, res;
		printf("Running command\n-------------------\n");
    pid = fork();
    switch(pid)
    {
    case -1:
			printf("There was an error creating the child process. Exiting...");
      exit(1);
    case 0:

			//'cd' case
			if (strcmp(cmd->args[0], "cd")==0) {

				if ((chdir(cmd->args[1]))==0){
					printf("change dir: success\n");
					exit(0);
				}
				else {
					// There is an error when the path has something different than
					// only lowercase letters. It returns -1 setting this else, but
					// chdir() executes succesfully nevertheless.
					// Seems like a bug from chdir()
					perror( cmd->args[1] );
					printf("change dir: FAILURE %s\n", cmd->args[1]);

					exit(EXIT_FAILURE);
				}
			}

			//Regular, execvp case
			else {

				// If is ECHOing an env variable
				if (strcmp (cmd->args[0], "echo")==0) {
					//cmd->args[1] = getenv(&cmd->args[1][1]);
					char c = cmd->args[1][0];
					//printf("char--> %d\n", c);
					if (c==36){
						cmd->args[1] = getenv(&cmd->args[1][1]);
					}
				}
				int redirection=0;
				for (int i=0; i<cmd->num_args; i++) {
					char c = cmd->args[i][0];
					if (c==62){
						redirection = i;
						break;
					}
				}
				//If there is redirection
				if (redirection) {
					// IF there is just one redirection
					if (cmd->num_args >= redirection+2) {
						int fd = open(cmd->args[redirection+1],O_WRONLY | O_CREAT,
													S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
						if (fd!=-1)
							printf("dup file handle: success\n");
						dup2 (fd, 1);
						close(fd);

					}

					if ((cmd->num_args > redirection+2) && strcmp (cmd->args[redirection+2], "2>&1") == 0){
						dup2 (1, 2);
						//close(1);
					}
					// We just want it to execute the commands before the redirection
					cmd->args[redirection] = NULL;

				}

				//printf("ENV var in Child SLE is %s\n", getenv("SLE"));
				res = execvp (cmd->args[0], cmd->args);
				//execl ("echo", "$SLE", NULL);

				//printf("%d\n", res);
				if (res == -1){
					printf("ERROR: There was an error executing the command: %s\n", cmd->args[0]);
					exit(EXIT_FAILURE);
				}
			}
      break;
    default:

      break;
    }
		if (pid) {

			wait(&stat_val);
			printf("-------------------\n");
      if(WIFEXITED(stat_val))
      	printf("Command Returned Exit Code %d\n", WEXITSTATUS(stat_val));
      else
        printf("Command terminated abnoramlly\n");
		}
	return(0);
}

/* Problem 3: set up all of your signal actions here */
int start_sig_catchers(void) {
  return(0);
}
