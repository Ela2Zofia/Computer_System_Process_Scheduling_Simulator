#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocate.h"

int main(int argc, char** argv){
    char* filename;
    int num_processor;
    int better_scheduler = 0;
    int time = 0;
    process* processes;
    process* tmp;
    int line_count;

    input_handler(argc, argv, &filename, &num_processor, &better_scheduler);
    
    printf("Filename: %s\nNumber of processor(s):%d\n",filename, num_processor);
    printf("Custom scheduler: %d\n",better_scheduler);

    processes = read_processes(filename);






    // free up spaces!
    tmp = processes;
    while(processes!=NULL){
        processes=processes->next;
        free(tmp);
        tmp=processes;
    }

}

void simulate(){
    
}

// handle command line inputs
void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler){
    for(int i = 0; i < argc; i++){
        
        if(!strcmp(argv[i],"-f")){
            *filename=argv[i+1];
        }

        if(!strcmp(argv[i],"-p")){
            *num_processor=atoi(argv[i+1]);
        }

        if(!strcmp(argv[i],"-c")){
            *better_scheduler=1;
        }
    }
}

// read processes specs from the file specified
process* read_processes(char* filename){


    
    FILE* input = fopen(filename,"r");
    process* processes;
    process* tmp;
    int time;
    int id;
    int exec_time;
    char parall;

    // read the specified file line by line, use of 'if' to keep track of
    // the head of the linked list.
    if (fscanf(input, "%d %d %d %c\n", &time, &id, &exec_time, &parall)){
        
        processes=(process*)malloc(sizeof(process));
        if (processes==NULL){
            printf("Malloc failure for processes.\n");
            exit(EXIT_FAILURE);
        }
        
        // value assignment
        processes->time_arrived = time;
        processes->processs_id = id;
        processes->execution_time = exec_time;
        if(parall == 'n'){
            processes->parallelisable = 0;
        }else if(parall == 'p'){
            processes->parallelisable = 1;
        }
        
        processes->next = NULL;
        tmp = processes;

        while(fscanf(input, "%d %d %d %c\n", &time, &id, &exec_time, &parall)!=EOF){
            tmp->next=(process*)malloc(sizeof(process));
            tmp = tmp->next;
            
            if (tmp==NULL){
                printf("Malloc failure for tmp.");
                exit(EXIT_FAILURE);
            }

            // value assignment
            tmp->time_arrived = time;
            tmp->processs_id = id;
            tmp->execution_time = exec_time;
            if(parall == 'n'){
                tmp->parallelisable = 0;
            }else if(parall == 'p'){
                tmp->parallelisable = 1;
            }

            tmp->next=NULL;
        }
    
    }else{
        printf("Invalid file input.\n");
        exit(EXIT_FAILURE);
    }

    fclose(input);

    return processes;
}

// push a new process to the CPU
void push(process** head, process** new_process){
    process* tmp = *head;
    if((*head)->execution_time > (*new_process)->execution_time){
        (*new_process)->next = *head;
        *head = *new_process;

    }else{
        while(tmp->next != NULL && tmp->next->execution_time < (*new_process)->execution_time){
            tmp = tmp->next;
        }
        (*new_process)->next=tmp->next;
        tmp->next = *new_process;
    }
}

// pop the head of the CPU queueueueueueueueueueue
void pop(process** head){
    process* tmp = *head;
    *head = (*head)->next;
    free(head);
}

// sum the remaining execution time of the given CPU
int time_sum(process* head){
    int time = 0;
    while(head != NULL){
        time += head->execution_time;
        head = head->next;
    }
}