#include <sys/types.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "potatoes.h"

int main(int argc, char * argv [] )
{
	int randval;
	struct potato spud;
	int msgid;
	int x;
	static int rotten, white, red;
	time_t t;
	int maxspuds = MAXSPUDS;

	if( argc > 1 )
	{
		maxspuds = atoi(argv[1]);
	}

	msgid = msgget((key_t) 73707564, 0666 | IPC_CREAT);
	if( msgid == -1 )
	{
		fprintf(stderr, "damn!  msgget failed\n");
		exit(EXIT_FAILURE);
	}

	time(&t);
	srand(t);
	printf("initializing seed to %d\n",t);
	sleep(5);
	for( x = 0; x < maxspuds; x++ )
	{
		spud.msg_type = 1;
		randval = rand();
		if ( randval == 0 )
			randval++;
		if( (randval % 5) == 0 )
		{
			spud.type = ROTTEN;
			rotten++;
		}
		else if( (randval % 2) == 0 )
		{
			spud.type = WHITE;
			white++;
		}
		else
		{
			spud.type = RED;
			red++;
		}
		if( msgsnd(msgid, (void *) &spud, sizeof(struct potato), 0) == -1 )
		{
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		else
			printf("added message index %d: spud.type is %d\n",x,spud.type);
	}
	printf("I created %d potatoes, of which:\n%d were white\n%d were red\nand %d were rotten\n",white+red+rotten,white,red,rotten);
}

