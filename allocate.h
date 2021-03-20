#include "pq.h"

void simulate(process** processes, int num_processor, int time);

void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler);

process* read_processes(char* filename);

int is_finished(process** cpu, int num_processor);

int time_sum(process* head);