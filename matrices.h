#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>

// ERROR MESSAGES 
#define DIMENSION_ERROR fprintf(stderr,"WARNING: DIMENSION_ERROR in function <%s> [%s:%d]\n\t Further computations may be undefined\n\n",__FUNCTION__,__FILE__,__LINE__)
#define ARGUMENT_ERROR fprintf(stderr,"WARNING: ARGUMENT_ERROR in function <%s> [%s:%d]\n\t Further computations may be undefined\n\n",__FUNCTION__,__FILE__,__LINE__)
#define NOT_IMPLEMENTED_ERROR fprintf(stderr,"WARNING: NOT_IMPLEMENTED_ERROR in function <%s> [%s:%d]\n\t Further computations may be undefined\n\n",__FUNCTION__,__FILE__,__LINE__)

#define ELEMENT_FORMAT " %.7f\t "

// MATRIX STRUCT 
typedef struct matrix {
    int nrows,ncols,row_step,col_step;
    double* values;
    int* rows;
    int* cols;
} *matrix_t;


//MATRIX CONSTRUCTORS
matrix_t matrix_init(int,int,double*);
matrix_t matrix_zeros(int,int);
matrix_t matrix_ones(int,int);
matrix_t matrix_eye(int,int);
matrix_t matrix_from_function(double(*)(int,int),int,int);

matrix_t matrix_deep_copy(matrix_t);
matrix_t matrix_slice(matrix_t in, int nrows, int ncols, int* rows, int* cols);
matrix_t matrix_shallow_copy(matrix_t);

void matrix_deep_delete(matrix_t);
void matrix_delete(matrix_t);

//MATRIX ALGEBRA
void matrix_transpose_inplace(matrix_t matrix);
matrix_t matrix_transpose(matrix_t matrix);
matrix_t matrix_transposed_view(matrix_t matrix);
matrix_t matrix_invert(matrix_t);

matrix_t matrix_add(matrix_t,matrix_t);
matrix_t matrix_product(matrix_t,matrix_t);


//ACCESSORS
double* matrix_entry(matrix_t,int,int);

// PRINTING
void matrix_print(FILE* stream, matrix_t);
void matrix_print_dbg(FILE* stream, matrix_t);
void matrix_print_gp(FILE* stream, matrix_t);



#endif
