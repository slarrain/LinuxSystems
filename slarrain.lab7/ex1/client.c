#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_server.h"


int main (int argc, char * argv[]) {

	int msgid;
	struct com up;

	msgid = msgget((key_t) 123456789, 0666 | IPC_CREAT);

	//Check for errors on  msgget
	if (msgid==-1)
	{
		fprintf(stderr, "Error on mssget\n");
		exit(EXIT_FAILURE);
	}

}
