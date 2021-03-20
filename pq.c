#include "pq.h"
#include <stdlib.h>

// push a new process to the CPU
void push(process** head, process* new_process){
    process* tmp = *head;
    
    if((*head)->execution_time > new_process->execution_time){
        
        new_process->next = *head;
        *head = new_process;

    }else{
        while(tmp->next != NULL && tmp->next->execution_time <= new_process->execution_time){
            
            // break tie favouring the process with smaller id when execution times are identical
            if (tmp->next->execution_time == new_process->execution_time && tmp->next->processs_id > new_process->processs_id){
                break;
            }
            
            tmp = tmp->next;
        }
        new_process->next=tmp->next;
        tmp->next = new_process;
    }
}

// pop the head of the CPU queueueueueueueueueueue
process* pop(process** head){
    process* tmp = *head;
    *head = (*head)->next;
    free(head);
    return tmp;
}
