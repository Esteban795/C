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
    bool* availables_times = malloc(sizeof(bool) * nb_tasks);
    for (int i = 0; i < nb_tasks; i++){
        availables_times[i] = true;
    }
    qsort(tasks,nb_tasks,sizeof(task),comp_tasks); //on suit juste l'heuristique (deadlines décroissantes)
    int nb_scheduled_tasks = 0;
    //Ici on place un maximum de tasks possibles
    for (int i = 0; i < nb_tasks; i++){
        for (int time = tasks[i].deadline - 1; time >= 0; time--){
            if (availables_times[time]) {
                tasks[i].start = time;
                availables_times[time] = false;
                nb_scheduled_tasks++;
                break;
            }
        }
    }
    //On s'occupe des tasks en retard
    int time = 0;
    int i = 0;
    while (nb_scheduled_tasks < nb_tasks){
        if (!availables_times[time]) time++;
        if (tasks[i].start != -1) i++; //la task est déjà scheduled donc on y touche pas
        else {
            tasks[i].start = time;
            availables_times[time] = false;
            nb_scheduled_tasks++;
            i++;
            time++;
        }
    }
    free(availables_times);
}