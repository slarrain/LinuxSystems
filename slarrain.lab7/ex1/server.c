#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <ctype.h>
#include <string.h>

#include "client_server.h"

int main (int argc, char * argv[])
{
	struct com up, down;
	int msgid_up, msgid_down, retval_up, retval_down;
	//char msg[256];

	msgid_up = msgget ( (key_t) 123456789, 0666 | IPC_CREAT);

	if( msgid_up == -1 )
	{
		fprintf(stderr, "Error on msgget\n");
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
			retval_up = msgrcv (msgid_up, (void *) &up, sizeof(struct com), 0, IPC_NOWAIT);

			switch (retval_up)
				{
				case -1:
						{
							continue;
						}
				default:
					{
						/*
						strcpy (down.message, up.message);
						//down.message = up.message;
						conv (down.message, 256);

						retval_down = msgsnd (msgid_down, (void *) &down, sizeof(struct com), 0);

						if (retval_down == -1)
							{
								fprintf (stderr, "Error on msgsnd\n");
								exit(EXIT_FAILURE);
							}

						 */


						conv (up.message, 256);

						retval_down = msgsnd (msgid_down, (void *) &up, sizeof(struct com), 0);

						if (retval_down == -1)
							{
								fprintf (stderr, "Error on msgsnd\n");
								exit(EXIT_FAILURE);
							}
					}
				}



		}


}

/* convert upper case to lower case or vise versa */
void conv(char *msg, int size)
{
     for (int i=0; i<size; ++i)
        if (islower(msg[i]))
            msg[i] =  toupper(msg[i]);
        else if (isupper(msg[i]))
            msg[i] =  tolower(msg[i]);
} 
