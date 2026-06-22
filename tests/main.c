#include<stdio.h>
#include "../c_core/ser_proc.h"

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

int test_tree(){
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

int main(){
    //test_hash();
    test_tree();
    return 0;
}

