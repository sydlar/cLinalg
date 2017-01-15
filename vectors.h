#ifndef VECTORS_H
#define VECTORS_H

#include "matrices.h"


// VECTOR STRUCT
typedef struct vector {
    int start,step,length;
    double* values;
} *vector_t;


//Vector CONSTRUCTORS
vector_t vector_init(int start ,int step ,int length,double* values);
vector_t vector_zeros(int length);
vector_t vector_from_function(double(*f)(int),int length);
void vector_delete(vector_t);
void vector_deep_delete(vector_t);

// VECTORS FROM MATRICES
vector_t matrix_row(matrix_t matrix, int i);
vector_t matrix_column(matrix_t matrix, int j);

// VECTOR OPERATIONS
void vector_elementwise_unary_op(double(*f)(double),vector_t in,vector_t out);

vector_t matrix_transform(matrix_t m,vector_t in);

double* vector_entry(vector_t,int);

void vector_print(FILE* stream, vector_t);

void vector_print_gp(FILE* stream, vector_t x, vector_t y);

#endif
