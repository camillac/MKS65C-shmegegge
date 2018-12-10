#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

char * get_line(char * line){
  fgets(line, 200, stdin);
  char * p = strchr(line, '\n');
  if (p) *p = 0;
  return line;
}

char * change(char * data){
  printf("Enter a string: ");
  return get_line(data);
}

int main(){
  key_t key = 12;
  int shmid;
  char * data;

  shmid = shmget(key, 200, 0644);
  if (shmid == -1){
    shmid = shmget(key, 200, 0644 | IPC_CREAT);
    printf("CREATED SHARED MEMORY\n");
  }
  data = shmat(shmid, (void *) 0, 0);
  if (data == (char *)(-1)){
    perror("shmat");
  }

  while (1){
    printf("Shared Memory: '%s'\n", data);

    printf("Would you like to change or delete the shared memory?\n");
    printf("Type 'change' to change or 'delete' to delete:\t");
    char * line = calloc(sizeof(char), 200);
    get_line(line);
    if (strcmp("change", line) == 0){
      change(data);
    }
    else if(strcmp("delete", line) == 0){
      shmctl(shmid, IPC_RMID, NULL);
    }
    printf("------------\n");
  }
  return 0;

}
