#include <iostream>
#include <string>
using namespace std;

int main() {
    string vdn_str1, vdn_str2;
    cout << "Enter first string: ";
    cin >> vdn_str1;
    cout << "Enter second string: ";
    cin >> vdn_str2;

    int vdn_len = vdn_str1.size();

    // DP table with extra row/col for base case
    int vdn_table[100][100] = {0};

    int vdn_max = 0;
    int vdn_endIndex = 0;

    // Fill DP table
    for (int vdn_i = 1; vdn_i <= vdn_len; vdn_i++) {
        for (int vdn_j = 1; vdn_j <= vdn_len; vdn_j++) {
            if (vdn_str1[vdn_i - 1] == vdn_str2[vdn_j - 1]) {
                vdn_table[vdn_i][vdn_j] = vdn_table[vdn_i - 1][vdn_j - 1] + 1;

                if (vdn_table[vdn_i][vdn_j] > vdn_max) {
                    vdn_max = vdn_table[vdn_i][vdn_j];
                    vdn_endIndex = vdn_i - 1;
                }
            } else {
                vdn_table[vdn_i][vdn_j] = 0;
            }
        }
    }

    // Print header row
    cout << "    ";
    for (int j = 0; j < vdn_len; j++) {
        cout << vdn_str1[j] << " ";
    }
    cout << endl;

    // Print table with row headers
    for (int i = 0; i <= vdn_len; i++) {
        if (i == 0) cout << "  ";
        else cout << vdn_str2[i - 1] << " ";

        for (int j = 0; j <= vdn_len; j++) {
            cout << vdn_table[i][j] << " ";
        }
        cout << endl;
    }

    // Extract substring
    string vdn_answer = vdn_str1.substr(vdn_endIndex - vdn_max + 1, vdn_max);
    cout << "\nLongest Common Substring: " << vdn_answer << endl;
    cout << "Length: " << vdn_max << endl;

    return 0;
}