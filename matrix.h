#ifndef MATRIX_H
#define MATRIX_H
#include <stdlib.h> 
#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
using namespace std;

class matrix{
    public:
    matrix(int rows, int cols);
    matrix(vector<int> arr);
    matrix mult(matrix B);
    void print();
    void randomize();
    int** A; 
    int rows;
    int cols;
};
struct MatrixBundle{
    matrix* A;
    matrix* B;
    matrix* C;
};
#endif