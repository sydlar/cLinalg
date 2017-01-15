#include "vectors.h"
#include <stdlib.h>
#include <stdio.h>

#define min(a,b) (a>b?b:a)
#define min(a,b) (a>b?b:a)

// UTILITY FUNCTIONS
void apply_unary_op(double(*f)(double),double* in, double* out,int n){
    for(int i = 0; i < n; i++)
        out[i] = f(in[i]);
}



// VECTOR CONSTRUCTORS
vector_t vector_init(int start,int step,int length ,double* values){
    vector_t new = malloc(sizeof(struct vector));
    new->start = start;
    new->step = step;
    new->length = length;
    new->values = values;
    return new;
}

vector_t vector_zeros(int length){
    return vector_init(0,1,length,calloc(sizeof(double),length));
}

vector_t vector_from_function(double(*f)(int),int length){
    vector_t vector = vector_zeros(length);
    for(int i =0; i < length;i++)
        *(vector_entry(vector,i)) = f(i);
    return vector;
}

void vector_delete(vector_t vector){
    free(vector);
}

void vector_deep_delete(vector_t vector){
    free(vector->values);
    vector_delete(vector);
}

// VECTORS FROM MATRICES
vector_t matrix_row(matrix_t matrix, int i){
    NOT_IMPLEMENTED_ERROR;
    return vector_init(0,0,matrix->ncols,matrix->values);
}

vector_t matrix_column(matrix_t matrix, int j){
    NOT_IMPLEMENTED_ERROR;
    return vector_init(0,0,matrix->nrows,matrix->values);
}


vector_t matrix_transform(matrix_t m,vector_t in){
    vector_t out = vector_zeros(in->length);
    for(int i = 0; i < out->length; i++){
        double value = 0.0;
        for(int j = 0; j < in->length; j++){
            value += *(vector_entry(in,j))**(matrix_entry(m,i,j));
        }
        *(vector_entry(out,i)) = value;
    }    
    return out;
}
// VECTOR OPERATIONS
void vector_elementwise_unary_op(double(*f)(double),vector_t in,vector_t out){
    if (in->length != out->length) DIMENSION_ERROR;
    apply_unary_op(f,in->values,out->values,in->length);
}

double* vector_entry(vector_t vector, int i ){
    if( i < 0 || i >= vector->length){DIMENSION_ERROR;}
    return vector->values + vector->start+i*vector->step;
}

void vector_print(FILE* stream, vector_t vector){
    fprintf(stream,"(");
    for(int i = 0; i < vector->length; i++){
        fprintf(stream,ELEMENT_FORMAT, *(vector_entry(vector,i)));
    }
    fprintf(stream,")\n");
}

void vector_print_gp(FILE* stream, vector_t x, vector_t y){
    int length = min(x->length,y->length);
    for(int i = 0; i < length; i++){
        fprintf(stream,"%f\t%f\n",*(vector_entry(x,i)),*(vector_entry(y,i)));
    }
}
