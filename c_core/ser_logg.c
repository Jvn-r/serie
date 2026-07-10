#include "cJSON.h"
#include "ser_proc.h"
#include "ser_logg.h"

static void timespec_add(struct timespec *a, struct timespec *b, struct timespec *resu){
    resu->tv_sec = b->tv_sec + a->tv_sec;
    resu->tv_nsec = b->tv_nsec + a->tv_nsec;

    if(resu->tv_nsec >= 1000000000L){
        resu->tv_nsec -= 1000000000L;
        resu->tv_sec++;
    }
}

static void timespec_sub(struct timespec *a, struct timespec *b, struct timespec *resu){
    resu->tv_sec = a->tv_sec - b->tv_sec;
    resu->tv_nsec = a->tv_nsec - b->tv_nsec;

    if(resu->tv_nsec < 0){
        resu->tv_nsec += 1000000000;
        resu->tv_sec--;
    }
}

int logg_time_reso(struct timespec *some_time, char *buff, size_t buff_size){
    struct timespec real_now, boot_now, boot_base, reso_time;
    if(clock_gettime(CLOCK_BOOTTIME, &boot_now) != 0)
        return -1;
    if(clock_gettime(CLOCK_REALTIME, &real_now) != 0)
        return -1;

    timespec_sub(&real_now, &boot_now, &boot_base);
    timespec_add(&boot_base, some_time, &reso_time);

    struct tm tm;
    localtime_r(&reso_time.tv_sec, &tm);

    char time_buff[32];
    if(strftime(time_buff, sizeof(time_buff), "%Y-%m-%d %H:%M:%S", &tm) == 0)
        return -1;

    snprintf(buff, buff_size, "%s.%09ld", time_buff, reso_time.tv_nsec);
    return 0;
}

void logg_proc_2_json(proc *p, cJSON *json){
    if(json == NULL)
        return;

    char bt_buff[64], dt_buff[64];
    size_t buff_size = 64;

    logg_time_reso(&p->birth_time, bt_buff, buff_size);
    logg_time_reso(&p->death_time, dt_buff, buff_size);

    cJSON_AddNumberToObject(json, "pid", p->pid);
    cJSON_AddNumberToObject(json, "real-uid", p->ruid);
    cJSON_AddNumberToObject(json, "effective-uid", p->euid);
    cJSON_AddNumberToObject(json, "read-gid", p->rgid);
    cJSON_AddNumberToObject(json, "effective-gid", p->egid);
    cJSON_AddStringToObject(json, "name", p->name);
    cJSON_AddNumberToObject(json, "parent-pid", p->ppid);
    cJSON_AddNumberToObject(json, "birth-parent-pid", p->birth_ppid);
    char state[2];
    sprintf(state, "%c", p->state);
    cJSON_AddStringToObject(json, "state", state);
    cJSON_AddNumberToObject(json, "thread-gid", p->tgid);
    cJSON_AddNumberToObject(json, "FD-count", p->fd_coun);
    cJSON_AddStringToObject(json, "birth-time", bt_buff);
    cJSON_AddStringToObject(json, "death-time", dt_buff);
    //add array of fd's 
    cJSON_AddBoolToObject(json, "alive", p->alive);
    cJSON_AddNumberToObject(json, "exit-code", p->exit_code);
    if(p->firs_chil != NULL)
        cJSON_AddNumberToObject(json, "first-child-pid", p->firs_chil->pid);
    if(p->next_sibl != NULL)
        cJSON_AddNumberToObject(json, "next-sibling-pid", p->next_sibl->pid);
    
}

static int logg_mem_writ(cJSON *j, char **json_buff, size_t *curr_len);

void logg_proc_parse(struct proc_tabl *table){
    //printf("INSIDE logg_proc_parse \n");
    char *json_buff = malloc(1);
    json_buff[0] = '\0';

    size_t curr_len = 0;

    for(int i = 0; i < 4096; i++){
        if(table->buckets[i] == NULL)
            continue; 
        proc_buck_node *curr = table->buckets[i];
        //printf("FIRST LOOP \n");
        while(curr != NULL){
            proc *p = curr->p;
            curr = curr->next;

            cJSON *j = cJSON_CreateObject();
            if(j == NULL)
                continue;

            logg_proc_2_json(p, j);
            //printf("logg_proc_2_json COMPLETED \n");

            if(logg_mem_writ(j, &json_buff, &curr_len) == -1){
                cJSON_Delete(j);
                continue;
            }

            //printf("%s INSIDE LOOP \n", json_buff);
            cJSON_Delete(j);
        }
    }
    //printf("%s OUTSIDE LOOP \n", json_buff);

    FILE *fp = fopen("/home/jean/me/codes/Serie/logs/data.ndjson", "a");
    if(fp == NULL){
        printf("FILLLLLLE BAD");
        free(json_buff);
        return;
    }
    fputs(json_buff, fp);
    fclose(fp);

    free(json_buff);

}

static int logg_mem_writ(cJSON *j, char **json_buff, size_t *curr_len){
    char *b = cJSON_PrintUnformatted(j);
    if(b == NULL)
        return -1;
    size_t blen = strlen(b);

    char *tmp = realloc(*json_buff, *curr_len + blen + 2);
    if(tmp == NULL){
        free(b);
        return -1;
    }

    *json_buff = tmp;

    strcpy(*json_buff + *curr_len, b);

    (*json_buff)[*curr_len + blen] = '\n';
    (*json_buff)[*curr_len + blen + 1] = '\0';
    *curr_len += blen + 1;

    //printf("%s INSIDE logg_mem_writ \n", *json_buff);
    free(b);
    return 0;
}
