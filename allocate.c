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
    process* temp;
    int line_count = 4;

    input_handler(argc, argv, &filename, &num_processor, &better_scheduler);
    
    printf("Filename: %s\nNumber of processor(s):%d\n",filename, num_processor);
    printf("Custom scheduler: %d\n",better_scheduler);

    processes = read_processes(filename);


    simulate(&processes, num_processor, time, line_count);


    // free up spaces!
    // tmp = processes;
    // while(processes!=NULL){
    //     processes=processes->next;
    //     free(tmp);
    //     tmp=processes;
    // }

}

void simulate(process** processes, int num_processor, int time, int line_count){
    // CPUs are represented as an array of linked lists
    process* cpu[num_processor];
    int current_id = -1;


    for (int i = 0; i < num_processor; i++){
        cpu[i] = NULL;
    }
    // process* tmp;
    // tmp = *processes;
    // while (tmp != NULL){
    //     printf("%d\n", tmp->processs_id);
    //     tmp = tmp->next;
    // }

    while((*processes)!=NULL || !is_finished(cpu, num_processor)){
        
        
        // single processor
        if (num_processor == 1){

            // check if the current process finishes
            if (cpu[0] != NULL){
                current_id = cpu[0]->processs_id;
                if (cpu[0]->execution_time > 1){
                    cpu[0]->execution_time--;
                }else{
                    process* finished = pop(&cpu[0]);
                    printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, finished->processs_id, line_count);
                    free(finished);
                    finished = NULL;
                }
                
            }

            // check if it is time for new process to arrive
            while ((*processes) != NULL && time == (*processes)->time_arrived){
                process* new_process = pop(processes);
                push(&cpu[0], new_process);
                line_count--;
                free(new_process);
                new_process = NULL;
            }

            // print out message when a new process is started or old one resumes
            if (cpu[0] != NULL && current_id != cpu[0]->processs_id){
                    printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=0\n", time, cpu[0]->processs_id, cpu[0]->execution_time);
            }
            
            time++;
            //printf("Time: %d\n",time);
        }else{
            break;
        }

    }

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

// check if all processes has been finished
int is_finished(process** cpu, int num_processor){
    int result = 1;
    for(int i = 0; i < num_processor; i++){
        if (cpu[i] != NULL){
            result = 0;
        }
    }
    return result;
}

// sum the remaining execution time of the given CPU
int time_sum(process* head){
    int time = 0;
    while(head != NULL){
        time += head->execution_time;
        head = head->next;
    }
}