#include "matrix.h"

struct MatrixBundle{
    matrix* A;
    matrix* B;
    matrix* C;
};
struct range{
    int rowS; int rowE;
    int colS; int colE;
};
struct MultParam{
    MatrixBundle* bund;
    range* Mrange;
};