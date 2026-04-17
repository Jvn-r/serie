#include<stdio.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<ctype.h>

static int lookup(){
  DIR *dp = opendir("/proc/");
  
  if(dp == NULL){
    perror("opendir err");
    return 1;
  }
  struct dirent *dirs;

  errno = 0;
  while((dirs = readdir(dp)) != NULL){
    if (strcmp(dirs->d_name,".") == 0 || strcmp(dirs->d_name,"..") == 0) 
      continue;

    if(isalpha(dirs->d_name[0]))
      continue;

    printf("%s\n", dirs->d_name);
  }

  if(errno != 0){
    perror("readdir error");
  }
  closedir(dp);
  return 0;
}

int main(){
  lookup();
  return 0;
}

