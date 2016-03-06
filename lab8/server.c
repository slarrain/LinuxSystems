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
#include <unistd.h>
//extern time_t time();

int check_word (char * buf);


//int maxlives = 6;

char * word[] = {
  #include "words"
};

// #define NUM_OF_WORDS (sizeof(word)/sizeof(word[0]))
#define NUM_OF_WORDS 45411
#define MAXBUF 250

int TCP_PORT = 1066;

int main(int argc, char * argv[])
{
  //int try = 0;
  int sock, fd;
  socklen_t client_len;
  struct sockaddr_in server, client;
  char buf[MAXBUF];
  char sendv[MAXBUF];


  if( argc > 1 )
    TCP_PORT = atoi(argv[1]);

  printf("working with port: %d\n",TCP_PORT);

  //srand((int)time((long *)0));

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if( sock < 0)
  {
    perror("creating stream socket");
    exit(1);
  }
  //Debugging purposes
  int yes = 1;
  if (setsockopt(sock, SOL_SOCKET, 	SO_REUSEADDR, &yes, sizeof(int)) < 0)
  {
    perror("setsockopt SO_REUSEADDR");
    exit(1);
  }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(TCP_PORT);

  if( bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0)
  {
    perror("binding socket");
    exit(2);
  }

  listen(sock, 5);

  client_len = sizeof(client);
  if ( (fd = accept(sock, (struct sockaddr *) &client, &client_len)) < 0)
  {
    perror("accepting connection");
    exit(3);
  }

  printf("Connected\n");


  while(1)
  {
    int a;
    a = read (fd, buf, MAXBUF);
    if (a<0)
    {
      perror ("Reading message");
      exit(1);
    }
    printf("Received %s", buf);


    // if ((retval = recv(fd, buf, 25, 0) ) < 0)
    // {
    //   perror ("Receiving message");
    //   exit (4);
    // }
    // printf("Received %s", buf);
    // sendv = check_word(buf);

    // strcpy (sendv, check_word(buf));
    // printf("Copy done %s - %s\n", sendv, buf);

    int found = check_word(buf);
    if (found){
      strcpy (sendv, "Word was Found\n");
    }
    else {
      strcpy (sendv, "Error, Word was NOT found\n");
    }

    //strcpy (sendv, buf);
    int b;
    b = write (fd, sendv, sizeof(sendv));
    if (b<0)
    {
      perror ("Writing message");
      exit(1);
    }

    printf("Sent %d\n", found);
    // if ((send(fd, sendv, strlen(sendv), 0) ) < 0)
    // {
    //   perror ("Sending message");
    //   exit (5);
    // }
    // play_hangman(fd, fd);
    //close(fd);
    // try++;
    // while ((try%10) != 0)
    // {
    //   printf ("Word: %s\n", word[try]);
    //   try++;
    // }

  }
}

int check_word (char * buf)
{
  for (int i=0; i<NUM_OF_WORDS; i++)
  {
    int j;
    j = strncmp (word[i], buf, sizeof(word[i]) -1);
    //printf ("word: %s %d buffer = %s | %d\n", word[i], j, buf, sizeof(char));
    if (j==0){
      return 1;
    }
  }
  return 0;
}
