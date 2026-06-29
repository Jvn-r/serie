#ifndef SER_PROC_CONN_H
#define SER_PROC_CONN_H
#define _GNU_SOURCE

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<linux/cn_proc.h>
#include<linux/netlink.h>
#include<linux/connector.h>
#include<errno.h>
#include<stdint.h>

typedef struct proc_tabl table;

int sock_crea();

void sock_regi(int sock);

void sock_unre(int sock);

int sock_send(int sock, enum proc_cn_mcast_op op);

int sock_reci(struct proc_tabl *table, int sock);

void read_even(struct proc_tabl *table, struct proc_event *even);

int even_fork(struct proc_tabl *table, struct proc_event *even);

void even_exec(struct proc_tabl *table, struct proc_event *even);

int even_exit(struct proc_tabl *table, struct proc_event *even);

void even_comm(struct proc_tabl *table, struct proc_event *even);

void even_uid(struct proc_tabl *table, struct proc_event *even);

void even_gid(struct proc_tabl *table, struct proc_event *even);

#endif
