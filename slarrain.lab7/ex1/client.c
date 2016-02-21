#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "client_server.h"


int main (int argc, char * argv[]) {

	int msgid_up, msgid_down, ret_msg_up, retmsg_down;
	struct com up, down;
	int pid;

	msgid_up = msgget((key_t) 123456789, 0666 | IPC_CREAT);

	//Check for errors on  msgget
	if (msgid_up==-1)
	{
		fprintf(stderr, "Error on msgget UP\n");
		exit(EXIT_FAILURE);
	}


	msgid_down = msgget ((key_t) 987654321, 0666 | IPC_CREAT);

	//Check for errors on  msgget
	if (msgid_down==-1)
	{
		fprintf(stderr, "Error on msgget DOWN\n");
		exit(EXIT_FAILURE);
	}

	while (1)
		{
			pid = getpid();
			up.msg_type = pid;
			printf("Insert message to send to server: ");
			fgets (up.message, 256, stdin);
			//printf ("Test long: %s\n", up.message);

			ret_msg_up = msgsnd (msgid_up, (void *) &up, sizeof(struct com), 0);

			if (ret_msg_up == -1)
				{
					fprintf (stderr, "Error on msgsnd\n");
					exit(EXIT_FAILURE);
				}
			//sleep(1);
			retmsg_down = msgrcv (msgid_down, (void *) &up, sizeof(struct com), pid, MSG_NOERROR);

			switch (retmsg_down)
				{
				case -1:
						{
							continue;
						}
				default:
						{
							printf ("Msg processed: %s\n", up.message);
						}
				}

		}

}
