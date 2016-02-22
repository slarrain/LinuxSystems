#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/*
* Shared memory server.
*/

int main(int argc, char *argv[])
{
  int shmid;
  char *shm;
  key_t key = IPC_PRIVATE;
  int flag = SHM_R | SHM_W;

  if (argc >= 2) {
    key = atoi(argv[1]);
  }

  shmid = shmget(key, 1000, flag);
  printf("shared memory for key %d: %d\n", key, shmid);
  if (shmid < 0) {
    perror("shmget");
    printf("Try to create this segment\n");
    shmid = shmget(key, 1000, flag | IPC_CREAT);
    if (shmid < 0) {
      perror("shmget | IPC_CREAT");
    }
  }

  shm = (char *)shmat(shmid, /*addr*/0, /*flag*/0); 
  if (shm == (char *)-1) {
    perror("shmat");
    exit(1);
  }
  sprintf(shm, "hello world\n");
  printf("shared memory content: %s\n", shm);
  sleep(1000);
  return 0;
} /* main */
