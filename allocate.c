#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void input_handler(int argc, char** argv, char** filename, int* num_processor, int* better_scheduler);


int main(int argc, char** argv){
    char* filename;
    int num_processor;
    int better_scheduler = 0;

    input_handler(argc, argv, &filename, &num_processor, &better_scheduler);
    
    printf("Filename: %s\nNumber of processor(s):%d\n",filename, num_processor);
    printf("Custom scheduler: %d\n",better_scheduler);
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