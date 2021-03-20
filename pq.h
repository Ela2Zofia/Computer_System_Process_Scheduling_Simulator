typedef struct process_t{
    int time_arrived;
    int processs_id;
    int execution_time;
    int parallelisable;
    struct process_t* next;
}process;

void push(process** head, process* new_process);

process* pop(process** head);