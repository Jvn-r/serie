#ifndef SER_PROC_H
#define SER_PROC_H

#define PROC_BUCK_NODES 4096

#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<sys/types.h>

typedef struct proc{
    pid_t pid;
    uid_t uid;
    gid_t gid;
    char name[16];
    pid_t ppid;
    char state;
    pid_t tgid;
    int fd_coun;
    struct timespec birth_time;
    struct timespec death_time;
    //char **fd_paths;
    bool alive; //1 = alive, 0 = dead
    int exit_code;
  
    struct proc *pare;
    struct proc *firs_chil;
    struct proc *next_sibl;
}proc;

/*
typedef struct pid_list{
  int *pids;
  int size;
}pid_list;
*/

typedef struct proc_buck_node{
    proc *p;
    struct proc_buck_node *next;
}proc_buck_node;

typedef struct proc_tabl{
    proc_buck_node **buckets;
    size_t bucket_count;
}proc_tabl;

int proc_tabl_init(proc_tabl *table);

int proc_inse(proc_tabl *table, proc *p);

proc *proc_look(proc_tabl *table, pid_t pid);

void proc_tabl_dest(proc_tabl *table);

int atta_chil(proc *pare, proc *chil);

int deta_proc(proc *chil);

#endif
