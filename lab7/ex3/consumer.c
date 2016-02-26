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
	sem_id = semget((key_t) 12345, 1 /* or 3 */, 0666 | IPC_CREAT );
	if( sem_id == -1 )
	{
		perror("semget failed: ");
		exit(EXIT_FAILURE);
	}

	/* initialize our new semaphore */
	sem_union.val = 0;  /* value for SETVAL */
	if( semctl(sem_id, 0, GETVAL, sem_union) == -1 )

	//unsigned short arr[3];
  //arr[0] = 1;
  //arr[1] = 300;
	//arr[2] = 1;
  //sem_union.array = arr;
	//if( semctl(sem_id, 0, GETALL, sem_union) == -1 )
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
			//sleep (0.1);

			//Defining CONSUMER
       //Code for the binary semaphore
				sop.sem_num = 0;			/* Specifies first semaphore in set */
				sop.sem_op = -1;		 /* Add, subtract, or wait for 0 */
				sop.sem_flg = 0;		/* No special options for operation */

				//Defining DECREMENT
       //Our EMPTY semaphore
				//sop.sem_num = 2;			/* Specifies first semaphore in set */
				//sop.sem_op = 1;		 /* Add, subtract, or wait for 0 */
				//sop.sem_flg = 0;		/* No special options for operation */

				//Defining DECREMENT
       //Our FULL semaphore
				//sop.sem_num = 1;			/* Specifies first semaphore in set */
				//sop.sem_op = -1;		 /* Add, subtract, or wait for 0 */
				//sop.sem_flg = 0;		/* No special options for operation */


			if (semop(sem_id, &sop, 1) == -1)
				{perror("semop");}


			if (*shelf>0)
				{
				--(*shelf);
				printf ("Removed 1 product from the shelf. Total: %d\n", *shelf);
				}

			sop.sem_num = 0;			/* Specifies first semaphore in set */
			sop.sem_op = 1;		 /* Add, subtract, or wait for 0 */
			sop.sem_flg = 0;		/* No special options for operation */
			//printf("%ld: about to semop at %s\n", (long) getpid(), currTime("%T"));
			if (semop(sem_id, &sop, 1) == -1)
				{perror("semop");}
		}

	return 0;
}
