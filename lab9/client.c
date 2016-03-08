#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>


#define MAXBUF 250


int main(int argc, char * argv[])
{
  int sock, result, TCP_PORT;
  //struct sockaddr_in server, client;
  char word_f[MAXBUF];
  //char word_b[MAXBUF];
  struct sockaddr_in address;


  if( argc != 3 )
	{
		printf("USAGE: %s hostname port\n",argv[0]);
		exit(-1);
	}

  TCP_PORT = atoi(argv[2]);

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if( sock < 0)
  {
    perror("creating stream socket");
    exit(1);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(argv[1]);
  address.sin_port = htons(TCP_PORT);
  //len = sizeof(address);

  result = connect(sock, (struct sockaddr *)&address, sizeof(address));
  if(result == -1) {
      perror("Error on client");
      exit(1);
  }
  printf("Enter word: ");
  while (fgets (word_f, MAXBUF, stdin) != NULL)
  {
    char *temp;
    temp = strtok (word_f, "\n");


    int w;
    w = write (sock, temp, sizeof(temp));
    if (w<0)
    {
      perror ("Writing message");
      exit(1);
    }

    // Added this for exiting the client
    if (strcmp (temp, "-exit")==0){
      printf("Exiting...\n");
      close(sock);
      exit(0);
    }

    int r, tf;
    r = read (sock, &tf, sizeof(int));
    if (r<0)
    {
      perror ("Reading message");
      exit(1);
    }

    if (tf) {
      printf ("The word %s is spelled correctly\n", temp);
    }
    else {
      printf ("The word %s is spelled incorrectly\n", temp);
      int r2;
      char list[10][MAXBUF];
      r2 = read (sock, list, sizeof(list));
      if (r2<0)
      {
        perror ("Reading message");
        exit(1);
      }
      for (int i=0; i<10; i++) {
        if (list[i][0]!='\0'){
          printf("%s\n", list[i]);
        }
      }

    }


    printf("\nEnter word: ");

   }
  //return 0;
}
