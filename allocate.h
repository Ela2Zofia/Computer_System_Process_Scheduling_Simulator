#include "pq.h"
#define MAX_PROCESS 100

void simulate(process** processes, int num_processor, int time, int line_count);

void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler);

process* read_processes(char* filename);

int is_finished(process** cpu, int num_processor);

int time_sum(process* head);