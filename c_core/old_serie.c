#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>

typedef struct process{
  pid_t pid;
  uid_t uid;
  gid_t gid;
  char name[16];
  pid_t ppid;
  char state;
  pid_t tgid;
  int fd_count;
  struct timespec birth_time;
  struct timespec death_time;
  char **fd_paths;
  int alive; //1 = alive, 0 = dead
  int exit_code;
  
  struct process *parent;
  struct process *first_child;
  struct process *next_sibling;
}p;

typedef struct pid_list{
  int *pids;
  int size;
}pid_list;

static int lookup(pid_list *out){
  DIR *dp = opendir("/proc/");
  
  if(dp == NULL){
    perror("opendir err");
    return 1;
  }
  struct dirent *dirs;
  int count = 0;
  int *pids = malloc(sizeof(int) * 1024);
  int cap = 1024;
  
  errno = 0;
  while((dirs = readdir(dp)) != NULL){
    if(!isdigit(dirs->d_name[0]))
      continue;
    
    if(count >= 1024){
      cap *= 2;
      pids = realloc(pids, sizeof(int) * cap);
    }

    pids[count] = atoi(dirs->d_name);
    count ++;
    //printf("%s\n", dirs->d_name);
  }

  if(errno != 0){
    perror("readdir error");
  }

  closedir(dp);
  out->pids = pids;
  out->size = count;

  return 0;
}

static int exists(pid_list *prev, pid_list *curr){

  return 0;
}

int main(){
  pid_list prev = {0}, curr = {0};
  
  lookup(&prev);

  while(1){
    //sleep();
    lookup(&curr);
    exists(&prev, &curr);
    if(prev.size != curr.size){
      printf("prev=%d curr=%d\n", prev.size, curr.size);
    }
    free(prev.pids);
    prev = curr;  
  }

  free(prev.pids);
  return 0;
}

