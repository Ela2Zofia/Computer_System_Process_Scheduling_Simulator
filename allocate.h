#include "pq.h"
#define MAX_PROCESS 100

// used to keep record of the processes that can create subprocesses
typedef struct multiprocess_t{
    int process_id;
    int subprocess;
    struct multiprocess_t* next;
    
}multiprocess;


void simulate(process** processes, int num_processor, int time, int line_count);

void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler);

process* read_processes(char* filename);

int is_finished(process** cpu, int num_processor);

int time_sum(process* head);