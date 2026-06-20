#include "ser_proc.h"

int proc_tabl_init(proc_tabl *table){
    table->bucket_count = PROC_BUCK_NODES;
    table->buckets = calloc(PROC_BUCK_NODES, sizeof(proc_buck_node *));
    if(!table->buckets)
        return -1;
    return 0;
}

static size_t proc_hash(pid_t pid){
    return pid & (PROC_BUCK_NODES - 1);
}

int proc_inse(proc_tabl *table, proc *p){
    size_t idx = proc_hash(p->pid);
    
    proc_buck_node *new_node = calloc(1,sizeof(proc_buck_node));
    if(!new_node){
        return -1;
    }

    new_node->p = p;
    new_node->next = table->buckets[idx];

    table->buckets[idx] = new_node;

    return 0;
}

proc *proc_look(proc_tabl *table, pid_t pid){
    size_t idx = proc_hash(pid);
    proc_buck_node *current = table->buckets[idx];

    while(current!=NULL){
        if(current->p->pid == pid){
            return current->p;
        }else{
            current = current->next;
        }
    }
    return NULL;
}

void proc_tabl_dest(proc_tabl *table){
    for(size_t i = 0; i < table->bucket_count; i++){
        proc_buck_node *current = table->buckets[i];
        while(current != NULL){
            proc_buck_node *next = current->next;
            free(current->p);
            free(current);
            current = next;
        }
    }
    free(table->buckets);
    free(table);
}
