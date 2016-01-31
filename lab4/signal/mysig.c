

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int n_sigint = 0;
int maxstops = 0;

void fsigint(int sig)
{
	n_sigint++;

  	if (maxstops == n_sigint) {
	  (void) signal (SIGINT, SIG_DFL);
	  }

  	printf("You've pressed Ctrl-C %d times.\n", n_sigint);

  	if (5<n_sigint && n_sigint<=15) {
	  printf ("You do realize this is useless, right?\n");
	  }
  	if (15<n_sigint && n_sigint<=30) {
	  printf ("OK. Keep it going...\n");
	  }
  	if (30<n_sigint && n_sigint<=50) {
	  printf ("Aren't you getting tired?\n");
	  }
  	if (50<n_sigint && n_sigint<=99) {
	  printf ("Can you make it up to a 100?\n");
	  }
  	if (n_sigint==100) {
	  printf ("You made it! But, I'm not going to stop\n");
	  }
  	if (100<n_sigint && n_sigint<=500) {
	  printf ("This is useless. I'm not going to stop\n");
	  }
  	if (500<n_sigint) {
	  printf ("You Sir, are a SIGINT Master\n");
	  }

}

void sigusr(int sig)
{
	printf("Roger that. The Signal SIGUSR1 has been received. \
			We will proceed accordingly \n");
}

void usage () {
  	fprintf(stderr, "Usage: ./mysig [MAXSTOPS]\n");
    exit(1);
}

int main(int argc, char *argv[])
{
	if (argc != 2 && argc != 1) {
       usage();
    }

  	if (argc==2) {
		int n = atoi(argv[1]);
	  	if (!(n>=0)) {
		  usage();
		  }
		else {
		  maxstops = n;
		}
	  }

	(void) signal(SIGINT, fsigint);
  	(void) signal (SIGUSR1, sigusr);


  	printf("Awaiting signals SIGINT or SIGUSR1\n");

	while(1)
	{
	  	//printf("\n");
		pause();
	}
}
