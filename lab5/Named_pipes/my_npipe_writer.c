#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char * argv[] )
{
	static char message[BUFSIZ];
	char * myfifo = "/tmp/mypipe";
	int fifo;

	/* create the FIFO, if it already exists, ignore the error */

	/*
	if( mkfifo(myfifo, 0666) < 0 )
	{
		printf("%s already exists, Using it\n",myfifo);
	}
	*/
	/* open the FIFO */
	if( (fifo = open(myfifo, O_WRONLY)) < 0)
	{
		perror("ERROR");
		exit(-1);
	}
	else
		printf("Opening named pipe: %s\n", myfifo);

	printf("Enter Input: ");
	while (( fgets(message,BUFSIZ,stdin) != NULL ) && (strcmp (message, "exit\n") != 0))
	{
		printf("Writing buffer to pipe...");
		write(fifo, message, strlen(message));
		printf("done\n");
		memset(message, '\0', BUFSIZ);
		printf("Enter Input: ");
	}
	printf("Exiting\n");
	close(fifo);
	exit(0);

}

