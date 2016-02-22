#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int shmid, counter;
	int * shelf;

	shelf = &counter;


	shmid = shmget ((key_t) 111111, sizeof(int), 0666 | IPC_CREAT);

	if( shmid < 0 )
	{
		perror("shmget failed\n");
		exit(EXIT_FAILURE);
	}

	shelf = (int *)shmat (shmid, NULL, 0);

	if (shelf == (int *)-1)
		{
     printf("*** shmat error (server) ***\n");
     exit(EXIT_FAILURE);
		}

	*shelf = 5;

	while (1)
		{
			//printf ("Shelf: %d\n", *shelf);
			if (*shelf<5){
				++(*shelf);
				printf ("Added 1 more prodcut to the shelf. Total: %d\n", *shelf);
			}
		}

	return 0;
}
