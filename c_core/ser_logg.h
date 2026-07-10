#ifndef SER_LOGG_H
#define SER_LOGG_H

#include<stdio.h>
#include "cJSON.h"
#include "ser_proc.h"
#include<time.h>

int logg_time_reso(struct timespec *some_time, char *buff, size_t buff_size);
void logg_proc_2_json(proc *p, cJSON *json);
void logg_proc_parse(struct proc_tabl *table);


#endif

