#include <iostream>
#include "matrix.h"
#include <time.h>
#include <iostream>


using namespace std;
const int N = 1000;


int main () {
    matrix A(N,N);
    matrix B(N,N);
    A.randomize(); B.randomize();
    //A.print(); cout<<endl; B.print(); cout<<endl;
    matrix C = A*B;
    //C.print();
    return 0;
}