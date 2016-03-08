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
#include <math.h>
#include <pthread.h>
#include <stdint.h>


int check_word (char * buf);
void suggest_words (char * buf);
int count_vowel (char * buf);
int check_vowel (char c);
void reset_list();

void * thread_func ();


pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
char list[10][250];
char * word[] = {
  #include "words"
};

#define NUM_OF_WORDS 45411
#define MAXBUF 250

int TCP_PORT = 1066;

int main(int argc, char * argv[])
{
  int sock, pt, fd;
  socklen_t client_len;
  struct sockaddr_in server, client;

  pthread_t t1;

  if( argc > 1 )
    TCP_PORT = atoi(argv[1]);

  printf("working with port: %d\n",TCP_PORT);


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

  while (1) {

    client_len = sizeof(client);
    if ( (fd = accept(sock, (struct sockaddr *) &client, &client_len)) < 0)
    {
      perror("accepting connection");
      exit(3);
    }

    printf("Connected fd %d\n", fd);

    pt = pthread_create (&t1, NULL, thread_func, (void *) (intptr_t)fd);
    if (pt!=0){

      perror("ERROR CREATING THREAD");
      exit(3);
    }

  }

}

int check_word (char * buf)
{
  for (int i=0; i<NUM_OF_WORDS; i++)
  {
    int j;
    j = strncmp (word[i], buf, sizeof(word[i]) -1);
    if (j==0){
      return 1;
    }
  }
  return 0;
}

void suggest_words (char * buf)
{
  int size = strlen(buf);
  char word[size];
  strcpy (word, buf);

  int k = 0;


  reset_list();

  for (int i = 0; i < size; i++){
    if (check_vowel(buf[i])) {

      for (int l=0; l<5; l++) {
        if (l==0){
          word[i] = 'a';
        }
        if (l==1){
          word[i] = 'e';
        }
        if (l==2){
          word[i] = 'i';
        }
        if (l==3){
          word[i] = 'o';
        }
        if (l==4){
          word[i] = 'u';
        }

        int exists = check_word (word);

        if (exists && (k<10)) {

          k++;
          strcpy (list[k], word);
        }
      }
      //Go back to the original one
      word[i] = buf[i];
    }
  }

}



int check_vowel (char c){
  if (c=='a' || c=='e' || c=='i' || c=='o' || c=='u') {
    return 1;
  }
  else {
    return 0;
  }
}

void reset_list() {
  for (int i=0; i<10; i++) {
    list[i][0] = '\0';
  }
}

int count_vowel (char * buf)
{
  int n_vowel = 0;
  int size = strlen(buf);
  for (int i=0; i<size; i++) {
    char c = buf[i];
    if (c=='a' || c=='e' || c=='i' || c=='o' || c=='u') {
      n_vowel++;
    }
  }
  return n_vowel;
}

void * thread_func (void * fd)
{
  char buf[MAXBUF];
  int fd2;
  fd2 = (int)(intptr_t) fd;
  printf("fd: %d\n", fd2);


  int det;
  det = pthread_detach(pthread_self());
  if (det!=0){
    //printf("Error %d\n", det);
    perror("ERROR DETACHING THREAD");
    exit(3);
  }


  while(1)
  {
    int a;
    a = read (fd2, buf, MAXBUF);
    if (a<0)
    {
      perror ("Reading message");
      exit(1);
    }
    printf("Received %s\n", buf);

    //Exits if it enters '-exit'
    // Closes the socket and closes the thread. Does not exit from
    // original parent thread (Ctl-C for that)
    if (strcmp (buf, "-exit")==0){
      //printf("Exiting...");
      close(fd2);
      return 0;
      //exit(0);
    }

    int found = check_word(buf);

    int b;
    b = write (fd2, &found, sizeof(int));
    if (b<0)
    {
      perror ("Writing message");
      exit(1);
    }

    if (!found) {

      // Mutex lock
      pthread_mutex_lock(&mymutex);
      suggest_words (buf);
      pthread_mutex_unlock(&mymutex);
      // Mutex unlocked

      int v;
      v = write (fd2, list, sizeof(list));
      if (v<0)
      {
        perror ("Writing message");
        exit(1);
      }
    }
  }


}
