
/*
This program will read in amatrix.txt and bmatrix.txt and will multiply
them together using a matrix c in a shared memory segment, which this
program creates, uses, detaches, but does NOT remove.  The reason it does
not remove the shared memory segment is because another process with this
program may be writing to the segment as well.

The shared memory matrix c created by this program may be dumped with
the shmdump program.

This program IS set up to run multiple versions in a divide and
conquer scenario.

This program demonstrates the additional use of semaphores
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "shmem.matrix.h"

#ifdef _SEM_SEMUN_UNDEFINED  /* cf. /usr/include/bits/sem.h */
union semun {
   int val;       /* value for SETVAL */
   struct semid_ds *buf;      /* buffer for IPC_STAT & IPC_SET */
   unsigned short *array;     /* array for GETALL & SETALL */
   struct seminfo *__buf;     /* buffer for IPC_INFO */
   void *__pad;
};
#endif

FILE * fp1;
FILE * fp2;

int shmid;
int sem_id;
union semun sem_union;

struct matrix_struct a; /* will be read in from amatrix.txt */
struct matrix_struct b; /* will be read in from bmatrix.txt */

void buildarray(char * filename, FILE * fp, matrix * m)
{
	int row = 0,col = 0,k = 0;
	int newval;
	int token;

	if( (fp = fopen(filename, "r")) == NULL )
	{
		perror("open: ");
		exit(EXIT_FAILURE);
	}
	m->cols = 0;
	m->rows = 0;

	while( ! feof(fp) )
	{
		fscanf(fp, "%d", &newval);
		fscanf(fp, "%c", &token);
		if(token == '\t')
		{
			m->mat[row][col++] = newval;
		}
		else if( token == '\n' )
		{
			if( m->cols < (col + 1) )
				m->cols = col + 1;
			m->mat[row++][col++] = newval;
			col = 0;
		}
		else
		{
			printf("fatal error: unknown token\n");
			exit(EXIT_FAILURE);
		}
	}
	m->rows = row - 1;
	fclose(fp);
}

int mult_matrix(matrix * c, matrix * a, matrix * b, int startrow, int endrow)
{
	unsigned short row,col,k;
	double x,y,z;
	if(a->cols != b->rows)
		return 0;
	c->rows = a->rows;
	c->cols = b->cols;

	printf("in mult_matrix, startrow is: %d and endrow is: %d\n",startrow,endrow);
	sleep(5);

	for( row = startrow; row < endrow; row++ )
	{
		for( col = 0; col < b->cols; col++ )
		{
			/* START of CRITICAL SECTION */
			struct sembuf sem_b;
			sem_b.sem_num = 0;
			sem_b.sem_op = -1;  /* passeren: wait, attempt to lock */
			sem_b.sem_flg = SEM_UNDO;  /* to handle process that exits w/o release */
			if( semop(sem_id, &sem_b, 1) == -1 )
			{
				perror("semop P failed\n");
				exit(EXIT_FAILURE);
			}
			/* CRITICAL SECTION LOCKED */
			c->mat[row][col] = 0.0;
			for( k = 0; k < a->cols; k++ )
			{
				c->mat[row][col] = c->mat[row][col] + (a->mat[row][k] * b->mat[k][col]);
/*				printf("%.0f ",(a->mat[row][k] * b->mat[k][col])); */
				switch(k)
				{
					case 0: x = (a->mat[row][k] * b->mat[k][col]);
					break;
					case 1: y = (a->mat[row][k] * b->mat[k][col]);
					break;
					case 2: z = (a->mat[row][k] * b->mat[k][col]);
					break;
				}
			}
			/* END of CRITICAL SECTION */
			sem_b.sem_num = 0;
			sem_b.sem_op = 1; /* vrijgeven:  signal, unlock */
			sem_b.sem_flg = SEM_UNDO;
			if( semop(sem_id, &sem_b, 1) == -1 )
			{
				perror("semop V failed\n");
				exit(EXIT_FAILURE);
			}
/*			printf(" = %.0f\n",x+y+z); */
		}
		printf("processed row %d\n",row);
	}
	return 1;
}

int main(int argc, char * argv[])
{
	matrix * c;
	int srow,erow;
	int row,col;
	struct sembuf sem_b;

	if( argc != 3 )
	{
		printf("USAGE:  %s startrow endrow\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	srow = atoi(argv[1])-1;
	erow = atoi(argv[2]);

	buildarray("amatrix.txt",fp1,&a);
	buildarray("bmatrix.txt",fp2,&b);

	/* now we need to create our semaphore.  We're only using 1 */
	sem_id = semget(ftok(".",1), 1, 0666 | IPC_CREAT );
	if( sem_id == -1 )
	{
		perror("semget failed: ");
		exit(EXIT_FAILURE);
	}

	/* initialize our new semaphore */
	sem_union.val = 1;  /* value for SETVAL */
	if( semctl(sem_id, 0, SETVAL, sem_union) == -1 )
	{
		perror("semctl failed on initialize: ");
		exit(EXIT_FAILURE);
	}

	/* after buildarray, [a,b].cols is the # of cols and [a,b].rows is # rows */
	/* START of CRITICAL SECTION */
	sem_b.sem_num = 0;
	sem_b.sem_op = -1;  /* passeren: wait */
	sem_b.sem_flg = SEM_UNDO;
	if( semop(sem_id, &sem_b, 1) == -1 )
	{
		perror("semop P failed\n");
		exit(EXIT_FAILURE);
	}
	/* CRITICAL SECTION LOCKED */

	shmid = shmget((key_t) 12345678, sizeof(struct matrix_struct), IPC_CREAT | 0644 );
	if( shmid < 0 )
	{
		perror("shmget failed: ");
		exit(EXIT_FAILURE);
	}
	/* END of CRITICAL SECTION */
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;  /*  */
	sem_b.sem_flg = SEM_UNDO;
	if( semop(sem_id, &sem_b, 1) == -1 )
	{
		perror("semop P failed\n");
		exit(EXIT_FAILURE);
	}
	/* CRITICAL SECTION UNLOCKED */

	c = (matrix *) shmat(shmid,NULL,0);
	if( c == (void *) -1 )
	{
		perror("shmat failed: ");
		exit(EXIT_FAILURE);
	}

	mult_matrix(c,&a,&b,srow,erow);

	#ifdef DONTDO
	/* out of our critical section, we can delete the semaphore now */
	if( semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
	{
		perror("semctl IPC_RMID failed: ");
		exit(EXIT_FAILURE);
	}
	#endif

	if ( shmdt(c) == -1 )
	{
		perror("shmdt failed: ");
		exit(EXIT_FAILURE);
	}

	printf("created shared memory segment with id: %d\n",shmid);
	exit(EXIT_SUCCESS);
}

