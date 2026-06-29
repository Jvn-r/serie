#ifndef SER_PRFS_H
#define SER_PRFS_H

#include<stdio.h>
#include<string.h> 
#include<stdlib.h> 
#include<stdbool.h>
#include<sys/types.h>
#include<limits.h>
#include<dirent.h>
#include<unistd.h>

typedef struct proc proc;

int prfs_look(proc *p);

int prfs_stat(proc *p);

int prfs_status(proc *p);

int prfs_fd(proc *p);

void prfs_clean_fd(proc *p, int i);

#endif
