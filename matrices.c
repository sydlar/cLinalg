#include "matrices.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a,b) (a>b?b:a)
#define abs(a) (a>0?a:-a)

#define DIMENSION_CHECK2(A,B) if(A->ncols!= B->ncols || A->nrows!=B->nrows) DIMENSION_ERROR
#define DIMENSION_CHECK3(A,B,C) if(A->ncols!= B->ncols || A->nrows!=B->nrows || C->nrows!=A->nrows || C->ncols!=A->ncols) DIMENSION_ERROR


// MOST INITIALIZATION


matrix_t matrix_init_full(int nrows, int ncols, int* rows, int* cols, double* values){
    matrix_t new = (matrix_t)malloc(sizeof(struct matrix));
    new->ncols = ncols;
    new->nrows = nrows;
    new->values = values;
    new->rows = rows; 
    new->cols = cols; 
    return new;
}

matrix_t matrix_init(int nrows,int ncols,double* values){
    int* rows = (int*)malloc(sizeof(int)*nrows);
    int* cols = (int*)malloc(sizeof(int)*ncols);
    for(int i = 0; i < nrows; i++) rows[i] = i*ncols;
    for(int j = 0; j < ncols; j++) cols[j] = j;
    return matrix_init_full(nrows,ncols,rows,cols,values);
}



// MATRIX CONSTRUCTORS
matrix_t matrix_zeros(int nrows, int ncols){
    return matrix_init(nrows,ncols,(double*)calloc(sizeof(double),nrows*ncols));
}

matrix_t matrix_ones(int nrows, int ncols){
    int n = nrows*ncols;
    double* array = (double*)malloc(sizeof(double)*n);
    for(int i = 0; i < n; i++)
        array[i] = 1.0;

    return matrix_init(nrows,ncols,array);
}

matrix_t matrix_from_function(double(*f)(int,int),int nrows,int ncols){
    matrix_t new = matrix_zeros(nrows,ncols);
    for(int i = 0; i < nrows; i++){
        for(int j=0; j < ncols; j++){
            *(matrix_entry(new,i,j)) = f(i,j);
        }
    }
    return new;
}

matrix_t matrix_eye(int nrows, int ncols){
    int n = min(nrows,ncols);
    matrix_t new = matrix_zeros(nrows,ncols);
    for(int i = 0; i < n; i++){
        *(matrix_entry(new,i,i)) = 1.0;
    }
    return new;
}


matrix_t matrix_deep_copy(matrix_t matrix){
    int bufferSize = matrix->ncols*matrix->nrows;
    double* newBuffer = malloc(sizeof(double)*bufferSize);
    matrix_t newMatrix = matrix_init(matrix->nrows,matrix->ncols,newBuffer);
    for(int i = 0; i < matrix->nrows; i++){
        for(int j = 0; j < matrix->ncols; j++){
            *matrix_entry(newMatrix,i,j) = *matrix_entry(matrix,i,j);
        }
    }
    return newMatrix;
}

matrix_t matrix_slice(matrix_t in, int nrows, int ncols, int* rows, int* cols){
    int* newRows = malloc(sizeof(int)*nrows);
    int* newCols = malloc(sizeof(int)*ncols);

    for(int j = 0; j < nrows; j++) newRows[j] = in->rows[rows[j]];
    for(int i = 0; i < ncols; i++) newCols[i] = in->cols[cols[i]];

    return matrix_init_full(nrows,ncols,newRows,newCols,in->values);
}

matrix_t matrix_shallow_copy(matrix_t matrix){
    int* cols = malloc(sizeof(int)*matrix->ncols);
    memcpy(cols,matrix->cols,sizeof(int)*matrix->ncols);
    int* rows = malloc(sizeof(int)*matrix->nrows);
    memcpy(rows,matrix->rows,sizeof(int*)*matrix->nrows);
    return matrix_init_full(matrix->nrows,matrix->ncols,rows,cols,matrix->values);
}

void matrix_deep_delete(matrix_t matrix){
    free(matrix->values);
    matrix_delete(matrix);
}

void matrix_delete(matrix_t matrix){
    free(matrix);
}



// ARRAY OPERATIONS
void array_binary_op(double(*f)(double,double),double* in_a, double* in_b,double* out,int n){
    for(int i = 0; i < n; i++)
        out[i] = f(in_a[i],in_b[i]);
}

void array_unary_op(double(*f)(double),double* in, double* out,int n){
    for(int i = 0; i < n; i++)
        out[i] = f(in[i]);
}

double add(double x, double y){return x+y;}
double neg(double x){return -x;}

// MATRIX OPERATIONS
void matrix_elementwise_binary_op(double(*f)(double,double),matrix_t in_a, matrix_t in_b,matrix_t out){
    DIMENSION_CHECK3(in_a,in_b,out);
    array_binary_op(f,in_a->values,in_b->values,out->values,in_a->nrows*in_a->ncols);
}
void matrix_elementwise_unary_op(double(*f)(double),matrix_t in, matrix_t out){
    DIMENSION_CHECK2(in,out);
    array_unary_op(f,in->values,out->values,in->nrows*in->ncols);
}

matrix_t matrix_add(matrix_t in_a, matrix_t in_b){
    matrix_t out = matrix_zeros(in_a->nrows,in_a->ncols);
    matrix_elementwise_binary_op(&add,in_a,in_b,out);
    return out;
}
void matrix_elementwise_neg(matrix_t in, matrix_t out){matrix_elementwise_unary_op(&neg,in,out);}


matrix_t matrix_product(matrix_t in_a, matrix_t in_b){
    if(in_a->ncols != in_b->nrows) DIMENSION_ERROR;
    matrix_t out = matrix_zeros(in_a->nrows,in_b->ncols);

    for(int i = 0; i < out->nrows; i++){
        for(int j = 0; j < out->ncols; j++){
            double value = 0.0;
            for(int k = 0; k < in_a->ncols; k++){
                value+=*(matrix_entry(in_a,i,k)) * *(matrix_entry(in_b,k,j));
            }
            *(matrix_entry(out,i,j))=value;
        }
    }
    return out;
}



void matrix_transpose_inplace(matrix_t matrix){
    int* tmp_ptr = matrix->rows;
    matrix->rows = matrix->cols;
    matrix->cols = tmp_ptr;

    int tmp_int = matrix->nrows;
    matrix->nrows = matrix->ncols;
    matrix->ncols = tmp_int;
}

// MATRIX ALGEBRA
matrix_t matrix_transposed_view(matrix_t in){
    matrix_t out = matrix_shallow_copy(in);
    matrix_transpose_inplace(out);
    return out;
}

matrix_t matrix_transpose(matrix_t in){
    matrix_t out = matrix_deep_copy(in);
    matrix_transpose_inplace(out);
    return out;
}



void row_scale(matrix_t m,int row,double s){
    for(int j = 0; j < m->ncols; j++){
        *matrix_entry(m,row,j) *= s;
    }
}

void row_elim(matrix_t m, int row, int oRow, double s){
    for(int j = 0; j < m->ncols; j++){
        *matrix_entry(m,row,j) += s * *matrix_entry(m,oRow,j);
    }
}

void row_swap(matrix_t m, int rowA, int rowB){
    int tmp = m->rows[rowA];
    m->rows[rowA] = m->rows[rowB];
    m->rows[rowB] = tmp;
}

#define size(i) abs(*matrix_entry(m,i,j))
int pivot_row(matrix_t m, int j){
    int max = j;
    for(int i = j+1; i < m->nrows; i++) if ((size(i)) > (size(max))) max = i;
    return max;
}

#define elem(i,j) *matrix_entry(tmp,i,j)
matrix_t matrix_invert(matrix_t in){
    if (in->nrows != in->ncols) DIMENSION_ERROR;
    int n = in->nrows;
    matrix_t out = matrix_eye(n,n);
    matrix_t tmp = matrix_deep_copy(in);

    for(int j = 0; j < n; j++){
        // PIVOTING
        int p = pivot_row(tmp,j);
        if (p!=j) {
            row_swap(tmp,p,j);
            row_swap(out,p,j);
        }

        //SCALING
        double scalar = 1.0 / *matrix_entry(tmp,j,j);
        row_scale(tmp,j,scalar);
        row_scale(out,j,scalar);
        
        //REDUCING
        for(int i = 0; i < n; i++){
            if (i == j) continue;
            double mult = -*matrix_entry(tmp,i,j);
            row_elim(tmp,i,j,mult);
            row_elim(out,i,j,mult);
        }
    }
    matrix_deep_delete(tmp);
    return out;
}




//ACCESSORS

double* matrix_entry(matrix_t matrix,int i, int j){
    if (i < 0 || i >= matrix->nrows || j < 0 || j >=matrix->ncols){DIMENSION_ERROR;}
    return matrix->values+matrix->rows[i]+matrix->cols[j];
}



// PRINTING
void matrix_print_helper(FILE* stream, matrix_t matrix,const char* format,const char* l,const char* r){
    for(int i = 0; i < matrix->nrows;i++){
        fprintf(stream,"%s",l);
        for(int j = 0; j < matrix->ncols; j++){
            fprintf(stream,format, *(matrix_entry(matrix,i,j)));
        }
        fprintf(stream,"%s\n",r);
    }
}

void matrix_print(FILE* stream, matrix_t matrix){
    matrix_print_helper(stream,matrix,"%.2f\t","|","|");
}


void matrix_print_dbg(FILE* stream, matrix_t matrix){
    fprintf(stream,"(ncols = %d, nrows = %d)\n",matrix->ncols,matrix->nrows);
    fprintf(stream,"ROW OFFSETS: ");
    for(int i = 0; i < matrix->nrows; i++)
        fprintf(stream," %d ",matrix->rows[i]);
    fprintf(stream,"\nCOL OFFSETS: ");
    for(int j = 0; j < matrix->ncols; j++)
        fprintf(stream," %d ",matrix->cols[j]);
    fprintf(stream,"\n");
}

void matrix_print_gp(FILE* stream, matrix_t matrix){
    matrix_print_helper(stream,matrix,"%.6f\t","","");
}

