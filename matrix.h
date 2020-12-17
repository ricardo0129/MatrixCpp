#ifndef MATRIX_H
#define MATRIX_H

class matrix{
    public:
    matrix(int rows, int cols);
    void mult(matrix B);

    private:
    int** A; 
};
#endif