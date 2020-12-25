#include "matrix.h"

const auto processor_count = thread::hardware_concurrency();
const int NUM_THREADS = 1; 

MatrixBundle* bundle;

struct range{
    int start;
    int end;
};

matrix::matrix(int rows, int cols){
    this->rows = rows;  this->cols = cols;
    A = (int**)malloc(rows*sizeof(int *));
    for(int i=0;i<rows;i++) 
        A[i] = (int*)malloc(cols*sizeof(int));
};


void 
matrix::print(){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<A[i][j]<<" ";
        }
        cout<<endl;
    }
}
void
*multiTwoMatrix(void* in){
    struct range* arg = (struct range*)in;
    //cout<<arg->start<<" "<<arg->end<<endl;
    for(int i=arg->start;i<arg->end;i++){
        for(int j=0;j<bundle->B->cols;j++){
            bundle->C->A[i][j] = 0;
            for(int k=0;k<bundle->A->cols;k++){
                bundle->C->A[i][j]+=bundle->A->A[i][k]*bundle->B->A[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

matrix
matrix::operator*(const matrix& B){
    matrix C(rows,B.cols);
    bundle = new struct MatrixBundle;
    bundle->A = this;
    bundle->B = (matrix *)&B;
    bundle->C = &C;
    int rc;
    int spacing = this->rows/NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for(int i = 0; i < NUM_THREADS; i++ ) {
        range* R = new struct range;
        R->start = i*spacing;
        R->end = (i+1)*spacing;
        rc = pthread_create(&threads[i], &attr, multiTwoMatrix, (void *)(R));
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for(int i = 0; i < NUM_THREADS; i++ ) {
        rc = pthread_join(threads[i], &status);
    }
    //pthread_exit(NULL);
    return C;
}

void
*addTwoMatrix(void* in){
    struct range* arg = (struct range*)in;
    for(int i=arg->start;i<arg->end;i++){
        for(int j=0;j<bundle->B->cols;j++){
            bundle->C->A[i][j] = bundle->A->A[i][j]+bundle->B->A[i][j];
        }
    }
    pthread_exit(NULL);
}

matrix
matrix::operator+(const matrix& B){
    matrix C(rows,B.cols);
    bundle = new struct MatrixBundle;
    bundle->A = this;
    bundle->B = (matrix *)&B;
    bundle->C = &C;
    int rc;
    int spacing = this->rows/NUM_THREADS;
    pthread_t threads[NUM_THREADS];
    pthread_attr_t attr;
    void *status;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for(int i = 0; i < NUM_THREADS; i++ ) {
        range* R = new struct range;
        R->start = i*spacing;
        R->end = (i+1)*spacing;
        rc = pthread_create(&threads[i], &attr, addTwoMatrix, (void *)(R));
    }

    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for(int i = 0; i < NUM_THREADS; i++ ) {
        rc = pthread_join(threads[i], &status);
    }
    //pthread_exit(NULL);
    return C;
}

void
matrix::randomize(){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            A[i][j]=rand()%100;
        }
    }
}