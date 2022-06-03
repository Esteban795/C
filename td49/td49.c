#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef int litteral;

struct cnf {
    litteral** clauses;
    int nb_clauses;
    int nb_variables;
};

typedef int sat;
typedef struct cnf cnf;

const sat SAT = 0;
const sat UNSAT = 1;
const sat UNKNOWN = 2;

sat remove_clause(cnf* f,int i){
    if (f->nb_clauses < 1) return;
    f->clauses[i][0] = 0; //clause non active
    f->nb_clauses--;
    if (f->nb_variables = 0) return SAT;
    else return UNKNOWN;
}

sat remove_litteral(litteral clause[],int index){
    clause[index] = 0;
    if (clause[0] = 0) return UNSAT;
    else return UNKNOWN;
}

int get_litteral_occurence(litteral clause[], litteral l){
    int count = 0;
    int n = clause[0];
    for (int i = 1; i < n; i++){
        if (clause[i] == l || clause[i] == -l) count++;
    }
    return count;
}

sat assert_litteral(cnf* f,litteral valuation[],litteral x){
    int count = 0;
}
