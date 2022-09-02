#ifndef TASK_H
#define TASK_H

#include <stdlib.h>

struct task {
    int id;
    int start;
    int penalty;
    int deadline;
};

typedef struct task task;


task* new_task(int id,int penalty, int deadline);
void print_task(task t);



#endif