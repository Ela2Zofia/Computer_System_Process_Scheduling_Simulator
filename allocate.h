typedef struct process_t{
    int time_arrived;
    int processs_id;
    int execution_time;
    int parallelisable;
    struct process_t* next;
}process;

void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler);

process* read_processes(char* filename);

void push(process** head, process** new_process);

void pop(process** head);

int time_sum(process* head);