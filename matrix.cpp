#include "matrix.h"
#include "bundle.h"

const auto processor_count = thread::hardware_concurrency();
const int NUM_THREADS = processor_count; 

void createThreads(matrix* A, matrix* B, matrix* C,void* (f)(void*)){
    MatrixBundle* bundle;
    bundle = new struct MatrixBundle;
    bundle->A = A;
    bundle->B = B;
    bundle->C = C;
    int rc;
    int N = A->rows*B->cols;
    int spacing = N/NUM_THREADS;
    int threadsCreated = min(NUM_THREADS,N);
    pthread_t threads[threadsCreated];
    pthread_attr_t attr;
    void *status;

    // Initialize and set thread joinable
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    for(int i=0; i<threadsCreated; i++) {
        range* R = new struct range;
        MultParam* P = new struct MultParam;
        int start = i*spacing; int end = min((i+1)*spacing,N-1);
        R->rowS = start/C->cols; R->colS = start%C->cols;
        R->rowE = end/C->cols; R->colE = end%C->cols;
        P->bund = bundle; P->Mrange = R;
        rc = pthread_create(&threads[i], &attr, f, (void*)P);
    }
    // free attribute and wait for the other threads
    pthread_attr_destroy(&attr);
    for(int i=0; i<threadsCreated; i++ ) {
        rc = pthread_join(threads[i], &status);
    }
    //pthread_exit(NULL);

}

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
    struct MultParam* arg = (struct MultParam*)in;
    struct range* R = (struct range*)arg->Mrange;
    matrix* A = arg->bund->A; matrix* B = arg->bund->B;
    matrix* C = arg->bund->C;
    int rs = arg->Mrange->rowS; int re = arg->Mrange->rowE;
    int cs = arg->Mrange->colS; int ce = arg->Mrange->colE;
    for(int i=rs;i<=re;i++){
        int l=0, r = C->cols;
        if(i==rs) l = cs;
        if(i==(arg->Mrange->rowE)) r = ce;
        for(int j=l;j<=r;j++){
            C->A[i][j] = 0;
            for(int k=0;k<A->cols;k++){
                C->A[i][j]+=A->A[i][k]*B->A[k][j];
            }
        }
    }
    pthread_exit(NULL);
}

matrix
matrix::operator*(const matrix& B){
    matrix* C;
    if(cols != B.rows){
        return *C;
    }
    C = new matrix(rows,B.cols);
    createThreads(this,(matrix *)&B,C,multiTwoMatrix);
    return *C;
}

void
*addTwoMatrix(void* in){
    struct MultParam* arg = (struct MultParam*)in;
    for(int i=arg->Mrange->rowS;i<=arg->Mrange->rowE;i++){
        int l=0, r = arg->bund->C->cols;
        if(i==arg->Mrange->rowS) l = arg->Mrange->colS;
        if(i==(arg->Mrange->rowE)) r = arg->Mrange->colE;
        for(int j=l;j<=r;j++){
            arg->bund->C->A[i][j] = arg->bund->A->A[i][j]+arg->bund->B->A[i][j];
        }
    }
    pthread_exit(NULL);
}

matrix
matrix::operator+(const matrix& B){
    matrix* C;
    if(rows != B.rows || cols != B.cols){
        return *C;
    }
    C = new matrix(rows,cols);
    createThreads(this,(matrix *)&B,C,addTwoMatrix);

    return *C;
}

void
matrix::randomize(){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            A[i][j]=rand()%100;
        }
    }
}

