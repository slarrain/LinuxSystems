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



	sem_id = semget((key_t) 12345, 1 /* or 3 */, 0666 | IPC_CREAT );
	if( sem_id == -1 )
	{
		perror("semget failed: ");
		exit(EXIT_FAILURE);
	}


	sem_union.val = 400;
	if( semctl(sem_id, 0, SETVAL, sem_union) == -1 )

		// For the 3 semaphores case
	//unsigned short arr[3];
  //arr[0] = 1;
  //arr[1] = 300;
	//arr[2] = 1;
  //sem_union.array = arr;
	//if( semctl(sem_id, 0, SETALL, sem_union) == -1 )
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

	//*shelf = 355;

	while (1)
		{

			//sleep(0.1);
			//printf ("Shelf: %d\n", *shelf);
			//if (*shelf<400){

				// Trying the array case from https://classes.cs.uchicago.edu/archive/2016/winter/51081-1/LabFAQ/lab7/Semaphores.html

				//Defining PRODUCER
        //Code for the binary semaphore
				//sop[0].sem_num = 0;			/* Specifies first semaphore in set */
				//sop[0].sem_op = -1;		 /* Add, subtract, or wait for 0 */
				//sop[0].sem_flg = 0;		/* No special options for operation */

				//Defining INCREMENT
       //Our FULL semaphore
				//sop[1].sem_num = 0;			/* Specifies first semaphore in set */
				//sop[1].sem_op = 1;		 /* Add, subtract, or wait for 0 */
				//sop[1].sem_flg = 0;		/* No special options for operation */

				//Defining INCREMENT
       //Our EMPTY semaphore
				//sop[2].sem_num = 0;			/* Specifies first semaphore in set */
				//sop[2].sem_op = -1;		 /* Add, subtract, or wait for 0 */
				//sop[2].sem_flg = 0;		/* No special options for operation */

				//Trying with the Built in array of semaphors
				//Defining PRODUCER
       //Code for the binary semaphore

				sop.sem_num = 0;			/* Specifies first semaphore in set */
				sop.sem_op = -1;		 /* Add, subtract, or wait for 0 */
				sop.sem_flg = 0;		/* No special options for operation */

				//Defining INCREMENT
       //Our FULL semaphore
				//sop.sem_num = 1;			/* Specifies first semaphore in set */
				//sop.sem_op = 1;		 /* Add, subtract, or wait for 0 */
				//sop.sem_flg = 0;		/* No special options for operation */

				//Defining INCREMENT
       //Our EMPTY semaphore
				//sop.sem_num = 2;			/* Specifies first semaphore in set */
				//sop.sem_op = -1;		 /* Add, subtract, or wait for 0 */
				//sop.sem_flg = 0;		/* No special options for operation */

				if (semop(sem_id, &sop, 1) == -1)
					{perror("semop");}


				if (*shelf<400)
					{
					++(*shelf);
					printf ("Added 1 more prodcut to the shelf. Total: %d\n", *shelf);
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
