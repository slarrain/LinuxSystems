/*
#
#		Santiago Larrain
#		slarrain@uchicago.edu
#
*/

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
	int n;


	/* create the FIFO, if it already exists, ignore the error */
	if( mkfifo(myfifo, 0666) < 0 )
	{
		fprintf(stderr,"%s already exists, Using it\n",myfifo);
	}
	else
		printf("Creating named pipe: %s\n", myfifo);

	/* open the FIFO */
	if( (fifo = open(myfifo, O_RDONLY)) < 0)
	{
		perror("ERROR");
		exit(-1);
	}


	while( ((n = read(fifo, message, sizeof(message))) > 0 ) && (strcmp (message, "exit\n") != 0))
	{
		printf("Waiting for input...");
		fprintf(stdout,"Got it: %s",message);
		memset(message, '\0', BUFSIZ);
	}

	printf("Exiting\n");
	close(fifo);
	exit(0);

}

