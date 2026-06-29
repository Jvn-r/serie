#include<stdio.h>
#include "../c_core/ser_proc.h"
#include "../c_core/ser_proc_conn.h"
#include "../c_core/ser_prfs.h"
#include <signal.h>

volatile sig_atomic_t running = 1;

void sigint_hand(int sig){
    (void)sig;
    running = 0;
}

/*
int test_hash(){
    printf("\n------HASH TEST---------\n");
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
    return 1;
}

void proc_tree_prin(proc *root, int depth){
    if(root == NULL)
        return;

    for(int i = 0; i < depth; i++)
        printf("    ");

    printf("%d\n", root->pid);

    proc *child = root->firs_chil;
    while(child != NULL){
        proc_tree_prin(child, depth + 1);
        child = child->next_sibl;
    }
}

int test_tree(){int proc_dele(proc_tabl *table, proc *p){
    size_t idx = proc_hash(p->pid);
    proc_buck_node *curr = table->buckets[idx];
    proc_buck_node *prev = NULL;
    while(curr != NULL){
        if(curr->p->pid == p->pid && curr->p->birth_time.tv_sec == p->birth_time.tv_sec && curr->p->birth_time.tv_nsec == p->birth_time.tv_nsec){
            if(prev == NULL)
                table->buckets[idx] = curr->next;
            else
                prev->next = curr->next;
            proc_dest(curr->p);
            free(curr);
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}
    printf("\n------TREE TEST---------\n");

    proc *P = calloc(1,sizeof(proc));
    proc *A = calloc(1,sizeof(proc));
    proc *B = calloc(1,sizeof(proc));
    proc *C = calloc(1,sizeof(proc));

    P->pid = 1;
    A->pid = 100;
    B->pid = 200;
    C->pid = 300;

    printf("\nATTACH TEST\n");
    atta_chil(P,A);
    atta_chil(P,B);
    atta_chil(P,C);

    proc_tree_prin(P,0);
    if(P->firs_chil == C)
        printf("P->firs_chil == C\n");
    if(C->next_sibl == B)
        printf("C->next_sibl == B\n");
    if(B->next_sibl == A)
        printf("B->next_sibl == A\n");
    if(A->next_sibl == NULL)
        printf("A->next_sibl == NULL\n");

    printf("\nDETACH HEAD\n");
    deta_proc(C);
    proc_tree_prin(P,0);
    if(P->firs_chil == B)
        printf("Head removal\n");
    if(C->pare == NULL && C->next_sibl == NULL)
        printf("C isolated\n");
    printf("\nREATTACH C\n");
    atta_chil(P,C);
    proc_tree_prin(P,0);

    printf("\nDETACH MIDDLE\n");
    deta_proc(B);
    proc_tree_prin(P,0);
    if(C->next_sibl == A)
        printf("Middle removal\n");
    if(B->pare == NULL && B->next_sibl == NULL)
        printf("B isolated\n");

    printf("\nDETACH TAIL\n");
    deta_proc(A);
    proc_tree_prin(P,0);
    if(C->next_sibl == NULL)
        printf("Tail removal\n");

    printf("\nSINGLE CHILD TEST\n");
    deta_proc(C);
    proc_tree_prin(P,0);
    if(P->firs_chil == NULL)
        printf("Empty child list\n");

    free(A);
    free(B);
    free(C);
    free(P);

    return 0;
}
*/

int test_proc_conn(){
    signal(SIGINT, sigint_hand);

    int sock = sock_crea();
    if(sock < 0)
        return -1;

    proc_tabl *table = calloc(1, sizeof(proc_tabl));
    if(!table){
        close(sock);
        return -1;
    }

    if(proc_tabl_init(table) < 0){
        free(table);
        close(sock);
        return -1;
    }

    sock_regi(sock);

    while(running){
        //sleep(1);
        sock_reci(table, sock);
    }

    sock_unre(sock);
    close(sock);
    proc_tabl_dest(table);
    return 0;
}

/*
int test_prfs(){
    proc *p = proc_load(getpid());
    printf("PID      : %d\n", p->pid);
    printf("PPID     : %d\n", p->ppid);
    printf("TGID     : %d\n", p->tgid);
    printf("State    : %c\n", p->state);
    printf("Name     : %s\n", p->name);
    printf("UID      : %d\n", p->ruid);
    printf("GID      : %d\n", p->rgid);
    printf("FD Count : %d\n", p->fd_coun);
    for(int i = 0; i < p->fd_coun; i++)
        printf("[%d] %s\n", i, p->fd_paths[i]);
    proc_dest(p);
    return 0;
}
*/
int main(){
    //test_hash();
    //test_tree();
    test_proc_conn();
    //test_prfs();
    return 0;
}

