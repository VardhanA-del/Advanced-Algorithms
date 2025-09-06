#include <iostream>
using namespace std;

int main() {
    int rowCount, colCount;

    cout << "No. of Rows & Columns:\n";
    cin >> rowCount >> colCount;

    // assume 10x10 matrix is max 
    int orgMat[10][10]; 

    // matrix values
    cout << "Enter the elements of the original matrix:";
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            cin >> orgMat[r][c];
        }
    }

    cout << "\nOriginal Matrix Representation:\n";
    for (int r = 0; r < rowCount; r++) {
        for (int c = 0; c < colCount; c++) {
            cout << orgMat[r][c] << " ";
        }
        cout << endl;
    }

    // sparse matrix representation
    int sparseMat[100][3];
    int nonZero = 0;

    // collect non-zero entries
    for (int a = 0; a < rowCount; a++) {
        for (int b = 0; b < colCount; b++) {
            if (orgMat[a][b] != 0) {
                sparseMat[nonZero][0] = a;
                sparseMat[nonZero][1] = b;
                sparseMat[nonZero][2] = orgMat[a][b];
                nonZero++;
            }
        }
    }

    // display sparse matrix
    cout << "Sparse Matrix (row, col, value):";
    cout << "\n";
    for (int k = 0; k < nonZero; k++) {
        cout << sparseMat[k][0] << " "
             << sparseMat[k][1] << " "
             << sparseMat[k][2] << endl;
    }

    return 0;
}