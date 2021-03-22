/*
By Jiawei(Derrick) Li, Student ID 1008207
Last modified: March 22nd, 2021
*/
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
    int line_count = 4;

    input_handler(argc, argv, &filename, &num_processor, &better_scheduler);
    
    // printf("Filename: %s\nNumber of processor(s):%d\n",filename, num_processor);
    // printf("Custom scheduler: %d\n",better_scheduler);

    processes = read_processes(filename);

    simulate(&processes, num_processor, time, line_count);

    return 0;

}


void simulate(process** processes, int num_processor, int time, int line_count){
    
    // CPUs are represented as an array of linked lists
    process* cpu[num_processor];
    double current_id[num_processor];
    int turnaround = 0;
    double overhead_sum = 0;
    double max_overhead = -1;
    int process_count = 0;

    for (int i = 0; i < num_processor; i++){
        cpu[i] = NULL;
        current_id[i] = -1; 
    }

    multiprocess* controller = NULL;

    // keep the simulation running as long as there are still new processes
    // or unfinished processes.
    while((*processes) != NULL || !is_finished(cpu, num_processor)){
        
        // single processor
        if (num_processor == 1){

            // check if a process is finished
            if (cpu[0] != NULL){
                current_id[0] = cpu[0]->processs_id;
                if (cpu[0]->remaining_time > 1){
                    cpu[0]->remaining_time--;
                }else{
                    process* finished = pop(&cpu[0]);
                    process_count--;
                    printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, (int)finished->processs_id, process_count);
                    
                    // calculate turnaround time and overhead
                    turnaround += (time - finished->time_arrived);
                    double overhead = (time - finished->time_arrived)/(double)finished->execution_time;
                    overhead_sum += overhead;
                    if (overhead > max_overhead){
                        max_overhead = overhead;
                    }
                    free(finished);
                    finished = NULL;
                }
                
            }

            // check if it is time for new process to arrive
            while ((*processes) != NULL && time == (*processes)->time_arrived){
                process* new_process = pop(processes);
                process_count++;
                push(&cpu[0], new_process);
                
                free(new_process);
                new_process = NULL;
            }

            // print out message when a new process is started or old one resumes
            if (cpu[0] != NULL && current_id[0] != cpu[0]->processs_id){
                printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=0\n", time, (int)cpu[0]->processs_id, cpu[0]->remaining_time);
            }
            
            time++;
        }
        // multiprocessors
        else{
            process* finished_processes = NULL;
            
            // check if a process is finished
            for(int i = 0; i < num_processor; i++){
                
                if (cpu[i] != NULL){
                    current_id[i] = cpu[i]->processs_id;

                    if (cpu[i]->remaining_time > 1){
                        cpu[i]->remaining_time--;
                    }else{
                        process* finished = pop(&cpu[i]);
                        
                        if (finished->parallelisable == 0){
                            
                            if (finished_processes == NULL){
                                finished_processes = finished;
                            }else{
                                process* tmp = finished_processes;
                                process* prev = finished_processes;
                                while(tmp!=NULL && tmp->processs_id < finished->processs_id){
                                    prev = tmp;
                                    tmp = tmp->next;
                                }
                                prev->next = finished;
                                finished->next = tmp;
                            }

                            process_count--;

                            // stats calculation
                            turnaround += (time - finished->time_arrived);
                            double overhead = (time - finished->time_arrived)/(double)finished->execution_time;
                            overhead_sum += overhead;
                            if (overhead > max_overhead){
                                max_overhead = overhead;
                            }
                        }else{
                            multiprocess* tmp = controller;
                            
                            while(tmp->process_id != (int)finished->processs_id){
                                tmp = tmp->next;
                            }

                            // check if all subprocesses of the parent process are finished, if yes, finish the process
                            if (tmp->subprocess > 1){
                                tmp->subprocess--;
                                free(finished);
                                finished = NULL;
                            }else{
                                if (finished_processes == NULL){
                                    finished_processes = finished;
                                }else{
                                    process* tmp = finished_processes;
                                    process* prev = finished_processes;
                                    while(tmp!=NULL &&  tmp->processs_id < finished->processs_id){
                                        prev = tmp;
                                        tmp = tmp->next;
                                    }
                                    prev->next = finished;
                                    finished->next = tmp;
                                }
                                
                                process_count--;

                                // stats calculation
                                turnaround += (time - finished->time_arrived);
                                double overhead = (time - finished->time_arrived)/(double)finished->execution_time;
                                overhead_sum += overhead;
                                if (overhead > max_overhead){
                                    max_overhead = overhead;
                                }
                            }
                        }
                        
                    }
                }
            }

            // print out all finished processes at this time stage
            while(finished_processes != NULL){
                process* finished = pop(&finished_processes);
                printf("%d,FINISHED,pid=%d,proc_remaining=%d\n", time, (int)finished->processs_id, process_count);
                free(finished);
                finished = NULL;
            }


            process* new_processes = NULL;

            // read in processes and sort them
            while ((*processes) != NULL && time == (*processes)->time_arrived){
                
                process* new = pop(processes);
                process* tmp = new_processes;
                process* prev = new_processes;
                process_count++;
                
                // sort the new processes
                sort(&new_processes, &new);
            }

            // assign processes to the CPUs
            while(new_processes!=NULL){
                
                process* new_process = pop(&new_processes);
                int min_time = -1;
                int cpu_time[num_processor];
                int cpu_order[num_processor];
                
                // find cpu with least time remaining
                for(int i = 0; i < num_processor; i++){
                    cpu_time[i] = time_sum(cpu[i]);
                    cpu_order[i] = -1;
                }
                
                int min_cpu = 0;
                // calculate the order of CPUs according to their remaining time
                for(int i = 0; i < num_processor; i++){
                    int local_min = __INT_MAX__;
                    for(int j = 0; j < num_processor; j++){
                        if (cpu_time[j] < local_min && cpu_time[j] >= min_time){
                            int existed = 0;
                            for (int x = 0; x < num_processor; x++){
                                if (cpu_order[x] == j){
                                    existed = 1;
                                }
                            }
                            if (!existed){
                                local_min = cpu_time[j];
                                min_cpu = j;
                            }
                            
                        }
                    }
                    cpu_order[i] = min_cpu;
                    min_time = local_min;
                }

                if (new_process->parallelisable == 0){
                    
                    push(&cpu[cpu_order[0]], new_process); 
                    
                }else{
                    int k = num_processor;
                    while(new_process->execution_time / k < 1){
                        k--;
                    }
                    
                    // create subprocesses
                    for(int i = 0; i < k; i++){
                        process* subprocess = malloc(sizeof(process));
                        subprocess->time_arrived = new_process->time_arrived;
                        subprocess->processs_id = new_process->processs_id + i*0.1;
                        subprocess->execution_time = new_process->execution_time;
                        subprocess->remaining_time = new_process->execution_time / k + (new_process->execution_time % k != 0) + 1;
                        subprocess->parallelisable = new_process->parallelisable;
                        subprocess->next = NULL;
                        push(&cpu[cpu_order[i]], subprocess);
                        free(subprocess);
                    }

                    // keep record of the status of parrallelisable processes
                    if (controller == NULL){
                        controller = malloc(sizeof(multiprocess));
                        controller->process_id = new_process->processs_id;
                        controller->subprocess = k;
                        controller->next=NULL;
                    }else{
                        
                        multiprocess* new_controller = malloc(sizeof(multiprocess));
                        new_controller->process_id=new_process->processs_id;
                        new_controller->subprocess = k;
                        new_controller->next = controller;
                        controller = new_controller;
                    }
                }

                free(new_process);
                new_process = NULL;

            }

            // check for running processes
            for(int i = 0; i < num_processor; i++){
                if (cpu[i] != NULL && current_id[i] != cpu[i]->processs_id){
                    if (cpu[i]->parallelisable == 0){
                        printf("%d,RUNNING,pid=%d,remaining_time=%d,cpu=%d\n", time, (int)cpu[i]->processs_id, cpu[i]->remaining_time, i);
                    }else{
                        printf("%d,RUNNING,pid=%.1f,remaining_time=%d,cpu=%d\n", time, cpu[i]->processs_id, cpu[i]->remaining_time, i);
                    }   
                }
            }
            
            time++;
        }

    }

    int t_time = turnaround / line_count + (turnaround % line_count != 0);

    printf("Turnaround time %d\n", t_time);
    printf("Time overhead %.2f %.2f\n", max_overhead+0.005, (overhead_sum/line_count)+0.005);
    printf("Makespan %d\n", time-1);

    // free up spaces!
    multiprocess* tmp;
    while (controller != NULL){
       tmp = controller;
       controller = controller->next;
       free(tmp);
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
        processes->remaining_time = exec_time;
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
            tmp->remaining_time = exec_time;
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
        time += head->remaining_time;
        head = head->next;
    }
    return time;
}
