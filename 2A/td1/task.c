#include <stdlib.h>
#include <stdio.h>


struct task {
    int id;
    int start;
    int penalty;
    int deadline;
};

typedef struct task task;

void print_task(task t){
    char* text;
    printf("Task %d (deadline %d) starts at time %d: %s,");
    if (t.start >= t.deadline) {
        printf("late, penalty %d",t.penalty);
    } else {
        printf("on time, penalty 0");
    }
}

task* new_task(int id,int penalty, int deadline){
    task* new_t = malloc(sizeof(task));
    new_t->deadline = deadline;
    new_t->id = id;
    new_t->penalty = penalty;
    new_t->start = -1;
}