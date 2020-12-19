#include <iostream>
#include "matrix.h"
#include <time.h>
#include <iostream>


using namespace std;
const int N = 2000;


int main () {
    matrix A(N,N);
    matrix B(N,N);
    A.randomize(); B.randomize();
    //A.print(); B.print();
    matrix C = A.mult(B);
    //C.print();

    return 0;
}