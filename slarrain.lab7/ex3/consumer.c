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
	int shmid;
	int * shelf;

	int sem_id;
	union semun sem_union;
	struct sembuf sop;		/* Structure defining operation */


	/* now we need to create our semaphore.  We're only using 1 */
	sem_id = semget((key_t) 12345, 1, 0666 | IPC_CREAT );
	if( sem_id == -1 )
	{
		perror("semget failed: ");
		exit(EXIT_FAILURE);
	}

	/* initialize our new semaphore */
	//sem_union.val = 15;  /* value for SETVAL */
	if( semctl(sem_id, 0, GETVAL, sem_union) == -1 )
	{
		perror("semctl failed on initialize: ");
		exit(EXIT_FAILURE);
	}

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
			sleep (1);
			/*
			if (*shelf > 0)
				{
					sop.sem_num = 0;
					sop.sem_op = *shelf;
					sop.sem_flg = 0;

					if (semop(sem_id, &sop, 1) == -1)
						{perror("semop");}

				}
			*/
			sop.sem_num = 0;			/* Specifies first semaphore in set */
			sop.sem_op = -1;		 /* Add, subtract, or wait for 0 */
			sop.sem_flg = 0;		/* No special options for operation */
			//printf("%ld: about to semop at %s\n", (long) getpid(), currTime("%T"));
			if (semop(sem_id, &sop, 1) == -1)
				{perror("semop");}

			--(*shelf);
			printf ("Removed 1 product from the shelf. Total: %d\n", *shelf);

		}

	return 0;
}
