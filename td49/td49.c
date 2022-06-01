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
    f->nb_clauses--;
    
}