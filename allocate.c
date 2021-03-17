#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct process_t{
    int time_arrived;
    int processs_id;
    int execution_time;
    int parallelisable;
    struct process_t* next;
}process;


void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler);
process* read_processes(char* filename);

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

    tmp = processes;
    while(tmp != NULL){
        printf("%d %d %d %d\n", tmp->time_arrived, tmp->processs_id, tmp->execution_time, tmp->parallelisable);
        tmp = tmp->next;
    }

    tmp = processes;
    while(processes!=NULL){
        processes=processes->next;
        free(tmp);
        tmp=processes;
    }

}

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

            tmp->time_arrived = time;
            tmp->processs_id = id;
            tmp->execution_time = exec_time;
            if(parall == 'n'){
                tmp->parallelisable = 0;
            }else if(parall == 'p'){
                tmp->parallelisable = 1;
            }

            //printf("%d %d %d %d\n", processes->time_arrived, processes->processs_id, processes->execution_time, processes->parallelisable);
            tmp->next=NULL;
        }
    
    }else{
        printf("Invalid file input.\n");
        exit(EXIT_FAILURE);
    }

    fclose(input);

    return processes;
}