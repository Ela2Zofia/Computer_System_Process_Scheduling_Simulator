#include "pq.h"
#include <stdlib.h>
#include <stdio.h>
// push a new process to the CPU
void push(process** head, process* new_process){
    process* tmp = (*head);
    process* prev = (*head);
    process* new = (process*)malloc(sizeof(process));

    new->time_arrived=new_process->time_arrived;
    new->processs_id=new_process->processs_id;
    new->execution_time=new_process->execution_time;
    new->remaining_time=new_process->remaining_time;
    new->parallelisable=new_process->parallelisable;
    new->next=NULL;
    
    //printf("Exe Time: %d\n", new->execution_time);

    if (!(*head)){
        *head=new;
    }else{
        
        if((*head)->remaining_time >= new->remaining_time){  
            if ((*head)->remaining_time == new->remaining_time && (*head)->processs_id > new->processs_id){
                new->next = *head;
                *head = new;
            }else{
                new->next = *head;
                *head = new;
            }
            
        }else{
            while(tmp != NULL && tmp->remaining_time <= new->remaining_time){

                // break tie favouring the process with smaller id when execution times are identical
                if (tmp->remaining_time == new->remaining_time && tmp->processs_id > new->processs_id){
                    break;
                }
                
                tmp = tmp->next;
                prev = tmp;
            }
            new->next=tmp;
            prev->next = new;
        }

    }
    
}

// pop the head of the CPU queueueueueueueueueueue
process* pop(process** head){
    process* tmp = *head;
    *head = (*head)->next;
    //printf("%d\n", (*head) == NULL);
    //free(head);
    return tmp;
}
