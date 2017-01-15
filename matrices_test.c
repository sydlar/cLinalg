#include <stdio.h>
#include <stdlib.h>
#include "matrices.h"
#include "vectors.h"
#include <math.h>

#define LINE printf("\n===================================\n\n");

#define NR 3
#define NC 2
#define N NR*NC


double f(int i, int j){
    double a = M_PI/3;
    if(i==j)
        return cos(a);
    if (i < j)
        return -sin(a*(i-j));
    if (i > j)
        return sin(a*(i-j));
    return 0;
}

double lin(int i){
    return 0.01*i;
}

double sin_i(int i){
    return sin(M_PI/100*i);
}

double cos_i(int i){
    return cos(M_PI/180*i);
}

void vector_test(int n){
    vector_t x = vector_from_function(&lin,n);
    vector_t y = vector_zeros(n);
    vector_elementwise_unary_op(cos,x,y);
    FILE* fp = fopen("TEST","w");
    vector_print_gp(fp,x,y);
    fclose(fp);
}

void matrix_inversion_test(){
    int K =3;
    matrix_t ma = matrix_from_function(&f,K,K);
    LINE;
    matrix_print_gp(stdout,ma);
    LINE;
    matrix_t ai = matrix_invert(ma);
    matrix_print(stdout,ai);
    LINE;
    matrix_t result = matrix_product(ma,ai);
    matrix_print(stdout,result);

}

void matrix_slice_test(){
    int K =4;

    int rows[] = {0,1,2};
    int cols[] = {1,2,3};

    matrix_t ma = matrix_from_function(&f,K,K);
    LINE;
    LINE;
    LINE;
    printf("MA:\n");
    matrix_print_dbg(stdout,ma);
    matrix_print(stdout,ma);
    LINE;
    printf("MAT:\n");
    matrix_t mat = matrix_slice(ma,3,3,rows,cols);
    matrix_print_dbg(stdout,mat);
    matrix_print(stdout,mat);

}

void matrix_testII(){
    double array[N];
    for(int i = 0; i < N ; i++){array[i] = i;}


    matrix_t ma = matrix_eye(NR,NC);
    matrix_t mb = matrix_init(NR,NC,array);
    matrix_t mc = matrix_from_function(&f,NR,NC);


    matrix_print(stdout,mc);
    
    LINE; 
    matrix_print(stdout,mb);
    LINE; 
    matrix_print(stdout,ma);
    LINE; 

    matrix_t test = mb;

    matrix_print(stdout,test);
    LINE; 


    for(int i = 0; i < NR; i++){vector_print(stdout,matrix_row(test,i));}
    LINE; 
    for(int j = 0; j < NC; j++){vector_print(stdout,matrix_column(test,j));}


    LINE; 
    LINE;
    matrix_print(stdout,mb);
    LINE;
    vector_t vector = vector_from_function(&cos_i,NC);
    
    vector_print(stdout,vector);
    vector_t out = vector_zeros(3);
    vector_t other = matrix_transform(mb,vector);
    vector_print(stdout,out);
}

#include "pthread_matrices.c"

void pthread_test(){
    eQueue_t q = eQueue_init();

    enqueue(q,2,3);
    enqueue(q,1,2);
    enqueue(q,2,3);
    enqueue(q,3,4);
    equeue_print(stdout,q);
}

int main(int argc, char** argv){
    //matrix_slice_test();
    pthread_test();
}

