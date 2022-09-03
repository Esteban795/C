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
    printf("Task %d (deadline %d) starts at time %d: ,",t.id,t.deadline,t.start);
    if (t.start >= t.deadline) {
        printf("late, penalty %d\n",t.penalty);
    } else {
        printf("on time, penalty 0\n");
    }
}

task* new_task(int id,int penalty, int deadline){
    task* new_t = malloc(sizeof(task));
    new_t->deadline = deadline;
    new_t->id = id;
    new_t->penalty = penalty;
    new_t->start = -1;
}