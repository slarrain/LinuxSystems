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
  char word_b[MAXBUF];
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
  //Debugging purposes
  // int yes = 1;
  // if (setsockopt(sock, SOL_SOCKET, 	SO_REUSEADDR, &yes, sizeof(int)) < 0)
  // {
  //   perror("setsockopt SO_REUSEADDR");
  //   exit(1);
  // }


  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(argv[1]);
  address.sin_port = htons(TCP_PORT);
  //len = sizeof(address);

  result = connect(sock, (struct sockaddr *)&address, sizeof(address));
  if(result == -1) {
      perror("Error: client1");
      exit(1);
  }

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
    printf("Sending %s to them\n",temp);

    int r;
    r = read (sock, word_b, MAXBUF);
    if (r<0)
    {
      perror ("Reading message");
      exit(1);
    }

    printf("Received back %s\n",word_b);

  //   printf("Sending word: %s = %ld", word, sizeof(word));
  //   //fflush(stdout);
  //
  //   if ((send(sock, word, /*strlen(word)*/25, 0) ) < 0)
  //   {
  //     perror ("Sending message");
  //     exit (2);
  //   }
  //
  //   if ((recv(sock, word, MAXBUF, 0) ) < 0)
  //   {
  //     perror ("Receiving message");
  //     exit (4);
  //   }
  //
  //   printf("Received word: %s\n", word);
  //
   }
  //return 0;
}
