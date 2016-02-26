#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
	int shmid;
	int * shelf;

	shmid = shmget ((key_t) 111111, sizeof(int), 0);
	printf("shmid %d\n", shmid);

	if( shmid < 0 )
	{
		perror("shmget failed\n");
		exit(EXIT_FAILURE);
	}

	// Attachment
	shelf = (int *)shmat (shmid, NULL, SHM_R | SHM_W);

	if (shelf == (int *)-1)
		{
     printf("*** shmat error (server) ***\n");
     exit(EXIT_FAILURE);
		}

	while (1)
		{
			//printf ("Shelf: %d\n", *shelf);
			if (0<*shelf){
				--(*shelf);

				printf ("Removed 1 prodcut from the shelf. Total: %d\n", *shelf);
			}
			else
				{
					printf ("No products on the shelf\n");
				}
		}

	return 0;
}
