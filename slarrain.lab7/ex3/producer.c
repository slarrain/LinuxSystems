#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>

#ifdef _SEM_SEMUN_UNDEFINED  /* cf. /usr/include/bits/sem.h */
union semun {
   int val;       /* value for SETVAL */
   struct semid_ds *buf;      /* buffer for IPC_STAT & IPC_SET */
   unsigned short *array;     /* array for GETALL & SETALL */
   struct seminfo *__buf;     /* buffer for IPC_INFO */
   void *__pad;
};
#endif


int main (int argc, char *argv[])
{
	int shmid, counter;
	int * shelf;

	int sem_id;
	union semun sem_union;

	struct sembuf sop;		/* Structure defining operation */



	sem_id = semget((key_t) 12345, 1, 0666 | IPC_CREAT );
	if( sem_id == -1 )
	{
		perror("semget failed: ");
		exit(EXIT_FAILURE);
	}


	sem_union.val = 5;
	if( semctl(sem_id, 0, SETVAL, sem_union) == -1 )
	{
		perror("semctl failed on initialize: ");
		exit(EXIT_FAILURE);
	}


	shelf = &counter;

	shmid = shmget ((key_t) 111111, sizeof(int), 0666 | IPC_CREAT);

	if( shmid < 0 )
	{
		perror("shmget failed\n");
		exit(EXIT_FAILURE);
	}

	// Atachment of share memory
	shelf = (int *)shmat (shmid, NULL, 0);
	//Check for errors on attachment
	if (shelf == (int *)-1)
		{
     printf("*** shmat error (server) ***\n");
     exit(EXIT_FAILURE);
		}

	*shelf = 5;

	while (1)
		{
			sleep(1);
			//printf ("Shelf: %d\n", *shelf);
			if (*shelf<15){

				sop.sem_num = 0;			/* Specifies first semaphore in set */
				sop.sem_op = 1;		 /* Add, subtract, or wait for 0 */
				sop.sem_flg = 0;		/* No special options for operation */

				if (semop(sem_id, &sop, 1) == -1)
					{perror("semop");}

				++(*shelf);
				printf ("Added 1 more prodcut to the shelf. Total: %d\n", *shelf);
			}
		}

	return 0;
}
