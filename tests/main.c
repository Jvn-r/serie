#include<stdio.h>
#include "ser_proc.h"

int main(){
    proc_tabl *table = calloc(1, sizeof(proc_tabl));
    proc_tabl_init(table);
    
    proc *A = calloc(1, sizeof(proc));
    A->pid = 100;
     
    proc *B = calloc(1, sizeof(proc));
    B->pid = 4196;
 
    proc *C = calloc(1, sizeof(proc));
    C->pid = 8292;

    if(proc_inse(table, A) == 0){
        printf("\n proc a inserted \n");
    }
    if(proc_inse(table, B) == 0){
        printf("\n proc B inserted \n");
    }
    if(proc_inse(table, C) == 0){
        printf("\n proc C inserted \n");
    }

    proc *a_look = proc_look(table, 100);
    if(a_look != NULL && a_look->pid == 100)
        printf("\n proc a LOOJED \n");

    proc *b_look = proc_look(table, 4196);
    if(b_look != NULL && b_look->pid == 4196)
        printf("\n proc b LOOJED \n");

    proc *c_look = proc_look(table, 8292);
    if(c_look != NULL && c_look->pid == 8292)
        printf("\n proc c LOOJED \n");
   
    proc *none_look = proc_look(table, 9999);
    if(none_look == NULL)
        printf("\n IT returned null \n");
    
    proc_tabl_dest(table);
    return 0;
}
