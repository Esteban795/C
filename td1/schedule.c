#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>


#include "schedule.h"


int comp_tasks(const void* t1,const void* t2){
    task* task1 = (task*)t1;
    task* task2 = (task*)t2;
    return task2->penalty - task1->penalty;
}


int total_penalty(task tasks[], int nb_tasks){
    int total = 0;
    for (int i = 0; i < nb_tasks; i++){
        assert(tasks[i].start >= 0);
        if (tasks[i].start >= tasks[i].deadline){
            total += tasks[i].deadline;
        }
    }
    return total;
}

void print_schedule(task tasks[],int nb_tasks){
    for (int i = 0; i < nb_tasks; i++){
        print_task(tasks[i]);
    }
    printf("\nTotal penalty : %d",total_penalty(tasks,nb_tasks));
}


void schedule(task tasks[],int nb_tasks){
    bool* instants_dispo = malloc(sizeof(bool) * nb_tasks);
    for (int i = 0; i < nb_tasks; i++){
        instants_dispo[i] = true;
    }
    qsort(tasks,nb_tasks,sizeof(task),comp_tasks);
}