#include <iostream>
using namespace std;

// node structure to store non-zero elements
struct Element {
    int rowIndex;      // row position
    int colIndex;      // column position
    int value;         // non-zero value
    Element* nextNode; // pointer to nxt node
};

// add a new node to linked list
void addNode(Element*& startNode, int row, int col, int val) {
    Element* newNode = new Element;
    newNode->rowIndex = row;
    newNode->colIndex = col;
    newNode->value = val;
    newNode->nextNode = NULL;

    if (startNode == NULL) {
        startNode = newNode; // 1st node
    } else {
        Element* tmp = startNode;
        while (tmp->nextNode != NULL) {
            tmp = tmp->nextNode;
        }
        tmp->nextNode = newNode; // new node at the end
    }
}

// linked list representation
void sparseMatrix(Element* startNode) {
    cout << "Sparse Matrix as Linked List:";
    cout << "\nRow  Col  Value  NextNodeAddress\n";
    Element* tmp = startNode;
    while (tmp != NULL) {
        cout << "  "<< tmp->rowIndex << "    "
             << tmp->colIndex << "    "
             << tmp->value << "    "
             << tmp->nextNode << endl; // print address of next node
        tmp = tmp->nextNode;
    }
}


int main() {
    int rows, cols;
    cout << "No. of Rows & Columns:\n";
    cin >> rows >> cols;

    int matrix[20][20]; // assuming fixed size
    cout << "Enter elements of the matrix (" << rows << " x " << cols << "):\n";
    for (int a = 0; a < rows; a++) {
        for (int b = 0; b < cols; b++) {
            cin >> matrix[a][b];
        }
    }

    cout << "\nOriginal Matrix:\n";
    for (int c = 0; c < rows; c++) {
        for (int d = 0; d < cols; d++) {
            cout << matrix[c][d] << " ";
        }
        cout << endl;
    }

    Element* startNode = NULL;

    // converting non-zero matrix elements into linked list
    for (int p = 0; p < rows; p++) {
        for (int q = 0; q < cols; q++) {
            if (matrix[p][q] != 0) {
                addNode(startNode, p, q, matrix[p][q]);
            }
        }
    }

    // sparse matrix linked list
    sparseMatrix(startNode);

    return 0;
}
